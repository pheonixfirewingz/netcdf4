#include "Group.h"
#include "Attribute.h"
#include "Dimension.h"
#include "Variable.h"
#include "nodenetcdfjs.h"
#include <netcdf.h>


namespace nodenetcdfjs
{

v8::Persistent<v8::Function> Group::constructor;

Group::Group(int id_) noexcept
    : id(id_)
{
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Object> obj =
        v8::Local<v8::Function>::New(isolate, constructor)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    Wrap(obj);
}

void Group::Init(v8::Local<v8::Object> exports)
{
    v8::Isolate *isolate = exports->GetIsolate();
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate);
    tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Group", v8::NewStringType::kNormal).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    NODE_SET_PROTOTYPE_METHOD(tpl, "addVariable", Group::AddVariable);
    NODE_SET_PROTOTYPE_METHOD(tpl, "addDimension", Group::AddDimension);
    NODE_SET_PROTOTYPE_METHOD(tpl, "addSubgroup", Group::AddSubgroup);
    NODE_SET_PROTOTYPE_METHOD(tpl, "addAttribute", Group::AddAttribute);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", Group::Inspect);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toJSON", Group::ToJSON);
    tpl->InstanceTemplate()->SetAccessor(
        v8::String::NewFromUtf8(isolate, "id", v8::NewStringType::kNormal).ToLocalChecked(), Group::GetId);
    tpl->InstanceTemplate()->SetAccessor(
        v8::String::NewFromUtf8(isolate, "variables", v8::NewStringType::kNormal).ToLocalChecked(),
        Group::GetVariables);
    tpl->InstanceTemplate()->SetAccessor(
        v8::String::NewFromUtf8(isolate, "dimensions", v8::NewStringType::kNormal).ToLocalChecked(),
        Group::GetDimensions);
    tpl->InstanceTemplate()->SetAccessor(
        v8::String::NewFromUtf8(isolate, "unlimited", v8::NewStringType::kNormal).ToLocalChecked(),
        Group::GetUnlimited);
    tpl->InstanceTemplate()->SetAccessor(
        v8::String::NewFromUtf8(isolate, "attributes", v8::NewStringType::kNormal).ToLocalChecked(),
        Group::GetAttributes);
    tpl->InstanceTemplate()->SetAccessor(
        v8::String::NewFromUtf8(isolate, "subgroups", v8::NewStringType::kNormal).ToLocalChecked(),
        Group::GetSubgroups);
    tpl->InstanceTemplate()->SetAccessor(
        v8::String::NewFromUtf8(isolate, "name", v8::NewStringType::kNormal).ToLocalChecked(), Group::GetName);
    tpl->InstanceTemplate()->SetAccessor(
        v8::String::NewFromUtf8(isolate, "fullname", v8::NewStringType::kNormal).ToLocalChecked(), Group::GetFullname);
    constructor.Reset(isolate, tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked());
}

bool Group::get_name(char *name) const noexcept
{
    const int retval = nc_inq_grpname(id, name);
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(v8::Isolate::GetCurrent(), retval);
        return false;
    }
    return true;
}

void Group::AddAttribute(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    auto *obj = node::ObjectWrap::Unwrap<Group>(args.Holder());

    if (args.Length() < 3)
    {
        isolate->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(isolate, "Wrong number of arguments", v8::NewStringType::kNormal)
                .ToLocalChecked()));
        return;
    }

    const std::string type_str = *v8::String::Utf8Value(isolate, args[1]);
    const int type = get_type(type_str);

    if (type == NC_NAT)
    {
        isolate->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(isolate, "Unknown variable type", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    auto *res = new Attribute(*v8::String::Utf8Value(isolate, args[0]), NC_GLOBAL, obj->id, type);
    res->set_value(args[2]);
    args.GetReturnValue().Set(res->handle());
}

void Group::AddSubgroup(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    auto *obj = node::ObjectWrap::Unwrap<Group>(args.Holder());

    if (args.Length() < 1)
    {
        isolate->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(isolate, "Wrong number of arguments", v8::NewStringType::kNormal)
                .ToLocalChecked()));
        return;
    }

    int new_id = -1;
    const int retval = nc_def_grp(obj->id, *v8::String::Utf8Value(isolate, args[0]), &new_id);

    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    auto *res = new Group(new_id);
    args.GetReturnValue().Set(res->handle());
}

void Group::AddDimension(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    auto *obj = node::ObjectWrap::Unwrap<Group>(args.Holder());

    if (args.Length() < 2)
    {
        isolate->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(isolate, "Wrong number of arguments", v8::NewStringType::kNormal)
                .ToLocalChecked()));
        return;
    }

    int len;
    if (const std::string len_str = *v8::String::Utf8Value(isolate, args[1]); len_str == "unlimited")
    {
        len = NC_UNLIMITED;
    }
    else
    {
        if (!args[1]->IsUint32())
        {
            isolate->ThrowException(v8::Exception::TypeError(
                v8::String::NewFromUtf8(isolate, "Expecting a positive integer", v8::NewStringType::kNormal)
                    .ToLocalChecked()));
            return;
        }
        len = args[1]->Uint32Value(isolate->GetCurrentContext()).ToChecked();
    }

    int new_id = -1;
    const int retval = nc_def_dim(obj->id, *v8::String::Utf8Value(isolate, args[0]), len, &new_id);

    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    auto *res = new Dimension(new_id, obj->id);
    args.GetReturnValue().Set(res->handle());
}

void Group::AddVariable(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    auto *obj = node::ObjectWrap::Unwrap<Group>(args.Holder());

    if (args.Length() < 3)
    {
        isolate->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(isolate, "Wrong number of arguments", v8::NewStringType::kNormal)
                .ToLocalChecked()));
        return;
    }

    const std::string type_str = *v8::String::Utf8Value(isolate, args[1]);
    const int type = get_type(type_str);

    if (type == NC_NAT)
    {
        isolate->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(isolate, "Unknown variable type", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    if (type == NC_STRING)
    {
        isolate->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(isolate, "Unsupported variable type", v8::NewStringType::kNormal)
                .ToLocalChecked()));
        return;
    }

    if (!args[2]->IsArray())
    {
        isolate->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(isolate, "Expecting an array", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    v8::Local<v8::Object> array = args[2]->ToObject(isolate->GetCurrentContext()).ToLocalChecked();
    const size_t ndims =
        array
            ->Get(isolate->GetCurrentContext(),
                  v8::String::NewFromUtf8(isolate, "length", v8::NewStringType::kNormal).ToLocalChecked())
            .ToLocalChecked()
            ->Uint32Value(isolate->GetCurrentContext())
            .ToChecked();

    // Use vector for automatic memory management
    std::vector<int> dimids(ndims);
    for (size_t i = 0; i < ndims; i++)
    {
        dimids[i] = array->Get(isolate->GetCurrentContext(), i)
                        .ToLocalChecked()
                        ->Int32Value(isolate->GetCurrentContext())
                        .ToChecked();
    }

    int new_id = -1;
    const int retval = nc_def_var(obj->id, *v8::String::Utf8Value(isolate, args[0]), type, static_cast<int>(ndims),
                                  dimids.data(), &new_id);

    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    auto *res = new Variable(new_id, obj->id);
    args.GetReturnValue().Set(res->handle());
}

void Group::GetId(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    const auto *obj = node::ObjectWrap::Unwrap<Group>(info.Holder());
    info.GetReturnValue().Set(v8::Integer::New(isolate, obj->id));
}

void Group::GetVariables(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    const auto *obj = node::ObjectWrap::Unwrap<Group>(info.Holder());

    int nvars = 0;
    int retval = nc_inq_varids(obj->id, &nvars, nullptr);
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    std::vector<int> var_ids(nvars);
    retval = nc_inq_varids(obj->id, nullptr, var_ids.data());
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    v8::Local<v8::Object> result = v8::Object::New(isolate);
    std::array<char, NC_MAX_NAME + 1> name{};

    for (int i = 0; i < nvars; ++i)
    {
        auto *v = new Variable(var_ids[i], obj->id);
        if (v->get_name(name.data()))
        {
            (void)result->CreateDataProperty(context,
                        v8::String::NewFromUtf8(isolate, name.data(), v8::NewStringType::kInternalized).ToLocalChecked(),
                        v->handle());
        }
        else
        {
            return;
        }
    }
    info.GetReturnValue().Set(result);
}

void Group::GetDimensions(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    const auto *obj = node::ObjectWrap::Unwrap<Group>(info.Holder());

    int ndims = 0;
    int retval = nc_inq_dimids(obj->id, &ndims, nullptr, 0);
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    std::vector<int> dim_ids(ndims);
    retval = nc_inq_dimids(obj->id, nullptr, dim_ids.data(), 0);
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    v8::Local<v8::Object> result = v8::Object::New(isolate);
    std::array<char, NC_MAX_NAME + 1> name{};

    for (int i = 0; i < ndims; ++i)
    {
        auto *d = new Dimension(dim_ids[i], obj->id);
        if (d->get_name(name.data()))
        {
            (void)result->CreateDataProperty(context,
                v8::String::NewFromUtf8(isolate, name.data(), v8::NewStringType::kInternalized).ToLocalChecked(),
                d->handle());
        }
        else
            return;
    }
    info.GetReturnValue().Set(result);
}

void Group::GetUnlimited(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    const auto *obj = node::ObjectWrap::Unwrap<Group>(info.Holder());

    int ndims = 0;
    int retval = nc_inq_unlimdims(obj->id, &ndims, nullptr);
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    std::vector<int> dim_ids(ndims);
    retval = nc_inq_unlimdims(obj->id, nullptr, dim_ids.data());
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    v8::Local<v8::Object> result = v8::Object::New(isolate);
    std::array<char, NC_MAX_NAME + 1> name{};

    for (int i = 0; i < ndims; ++i)
    {
        auto *d = new Dimension(dim_ids[i], obj->id);
        if (d->get_name(name.data()))
        {
             (void)result->CreateDataProperty(context,
                        v8::String::NewFromUtf8(isolate, name.data(), v8::NewStringType::kInternalized).ToLocalChecked(),
                        d->handle());
        }
        else return;
    }
    info.GetReturnValue().Set(result);
}

void Group::GetAttributes(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    const auto *obj = node::ObjectWrap::Unwrap<Group>(info.Holder());

    int natts = 0;
    int retval = nc_inq_natts(obj->id, &natts);
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    v8::Local<v8::Object> result = v8::Object::New(isolate);
    std::array<char, NC_MAX_NAME + 1> name{};

    for (int i = 0; i < natts; i++)
    {
        retval = nc_inq_attname(obj->id, NC_GLOBAL, i, name.data());
        if (retval != NC_NOERR)
        {
            throw_netcdf_error(isolate, retval);
            return;
        }
        auto *a = new Attribute(name.data(), NC_GLOBAL, obj->id);
        (void)result->CreateDataProperty(context,
                    v8::String::NewFromUtf8(isolate, name.data(), v8::NewStringType::kInternalized).ToLocalChecked(),
                    a->handle());
    }
    info.GetReturnValue().Set(result);
}

void Group::GetSubgroups(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    const auto *obj = node::ObjectWrap::Unwrap<Group>(info.Holder());

    int ngrps = 0;
    int retval = nc_inq_grps(obj->id, &ngrps, nullptr);
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    std::vector<int> grp_ids(ngrps);
    retval = nc_inq_grps(obj->id, nullptr, grp_ids.data());
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    v8::Local<v8::Object> result = v8::Object::New(isolate);
    std::array<char, NC_MAX_NAME + 1> name{};

    for (int i = 0; i < ngrps; ++i)
    {
        auto *g = new Group(grp_ids[i]);
        if (g->get_name(name.data()))
        {
             (void)result->CreateDataProperty(context,
                        v8::String::NewFromUtf8(isolate, name.data(), v8::NewStringType::kInternalized).ToLocalChecked(),
                        g->handle());
        }
        else
        {
            return;
        }
    }
    info.GetReturnValue().Set(result);
}

void Group::GetName(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    const auto *obj = node::ObjectWrap::Unwrap<Group>(info.Holder());

    std::array<char, NC_MAX_NAME + 1> name{};
    if (obj->get_name(name.data()))
    {
        info.GetReturnValue().Set(
            v8::String::NewFromUtf8(isolate, name.data(), v8::NewStringType::kNormal).ToLocalChecked());
    }
}

void Group::GetFullname(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    const auto *obj = node::ObjectWrap::Unwrap<Group>(info.Holder());

    size_t len = 0;
    int retval = nc_inq_grpname_len(obj->id, &len);
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    std::vector<char> name(len + 1, '\0');
    retval = nc_inq_grpname_full(obj->id, nullptr, name.data());
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }

    info.GetReturnValue().Set(
        v8::String::NewFromUtf8(isolate, name.data(), v8::NewStringType::kNormal).ToLocalChecked());
}

void Group::Inspect(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    args.GetReturnValue().Set(
        v8::String::NewFromUtf8(isolate, "[object Group]", v8::NewStringType::kNormal).ToLocalChecked());
}

void Group::ToJSON(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    const auto *obj = node::ObjectWrap::Unwrap<Group>(args.Holder());
    
    // Use internalized strings for better performance
    v8::Local<v8::String> id_str = v8::String::NewFromUtf8Literal(isolate, "id");
    v8::Local<v8::String> name_str = v8::String::NewFromUtf8Literal(isolate, "name");
    v8::Local<v8::String> fullname_str = v8::String::NewFromUtf8Literal(isolate, "fullname");
    v8::Local<v8::String> dimensions_str = v8::String::NewFromUtf8Literal(isolate, "dimensions");
    v8::Local<v8::String> variables_str = v8::String::NewFromUtf8Literal(isolate, "variables");
    v8::Local<v8::String> attributes_str = v8::String::NewFromUtf8Literal(isolate, "attributes");
    v8::Local<v8::String> subgroups_str = v8::String::NewFromUtf8Literal(isolate, "subgroups");
    
    v8::Local<v8::Object> json = v8::Object::New(isolate);
    
    // Add id
    (void)json->CreateDataProperty(context, id_str, v8::Integer::New(isolate, obj->id));
    
    // Add name
    std::array<char, NC_MAX_NAME + 1> name{};
    if (obj->get_name(name.data()))
    {
        (void)json->CreateDataProperty(context, name_str,
                  v8::String::NewFromUtf8(isolate, name.data(), v8::NewStringType::kInternalized).ToLocalChecked());
    }
    
    // Add fullname
    size_t len = 0;
    int retval = nc_inq_grpname_len(obj->id, &len);
    if (retval == NC_NOERR)
    {
        std::vector<char> fullname(len + 1, '\0');
        retval = nc_inq_grpname_full(obj->id, nullptr, fullname.data());
        if (retval == NC_NOERR)
        {
            (void)json->CreateDataProperty(context, fullname_str,
                      v8::String::NewFromUtf8(isolate, fullname.data(), v8::NewStringType::kInternalized).ToLocalChecked());
        }
    }
    
    // Add dimensions, variables, attributes, subgroups
    // These will be accessed through property getters which trigger the respective Get* methods
    v8::Local<v8::Value> dimensions = args.Holder()->Get(context, dimensions_str).ToLocalChecked();
    if (dimensions->IsObject())
    {
        (void)json->CreateDataProperty(context, dimensions_str, dimensions);
    }
    
    v8::Local<v8::Value> variables = args.Holder()->Get(context, variables_str).ToLocalChecked();
    if (variables->IsObject())
    {
        (void)json->CreateDataProperty(context, variables_str, variables);
    }
    
    v8::Local<v8::Value> attributes = args.Holder()->Get(context, attributes_str).ToLocalChecked();
    if (attributes->IsObject())
    {
        (void)json->CreateDataProperty(context, attributes_str, attributes);
    }
    
    v8::Local<v8::Value> subgroups = args.Holder()->Get(context, subgroups_str).ToLocalChecked();
    if (subgroups->IsObject())
    {
        (void)json->CreateDataProperty(context, subgroups_str, subgroups);
    }
    
    args.GetReturnValue().Set(json);
}
} // namespace nodenetcdfjs
