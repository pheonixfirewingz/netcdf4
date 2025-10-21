#include "Dimension.h"
#include "netcdf4js.h"
#include <netcdf.h>


namespace netcdf4js
{

v8::Persistent<v8::Function> Dimension::constructor;

Dimension::Dimension(int id_, int parent_id_) noexcept
    : id(id_)
    , parent_id(parent_id_)
{
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Object> obj =
        v8::Local<v8::Function>::New(isolate, constructor)->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
    Wrap(obj);
}

void Dimension::Init(v8::Local<v8::Object> exports)
{
    v8::Isolate *isolate = exports->GetIsolate();
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate);
    tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Dimension", v8::NewStringType::kNormal).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", Dimension::Inspect);
    tpl->InstanceTemplate()->SetAccessor(
        v8::String::NewFromUtf8(isolate, "id", v8::NewStringType::kNormal).ToLocalChecked(), Dimension::GetId);
    tpl->InstanceTemplate()->SetAccessor(
        v8::String::NewFromUtf8(isolate, "length", v8::NewStringType::kNormal).ToLocalChecked(), Dimension::GetLength);
    tpl->InstanceTemplate()->SetAccessor(
        v8::String::NewFromUtf8(isolate, "name", v8::NewStringType::kNormal).ToLocalChecked(), Dimension::GetName,
        Dimension::SetName);
    constructor.Reset(isolate, tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked());
}

bool Dimension::get_name(char *name) const noexcept
{
    const int retval = nc_inq_dimname(parent_id, id, name);
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(v8::Isolate::GetCurrent(), retval);
        return false;
    }
    return true;
}

void Dimension::GetId(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    const auto *obj = node::ObjectWrap::Unwrap<Dimension>(info.Holder());
    info.GetReturnValue().Set(v8::Integer::New(isolate, obj->id));
}

void Dimension::GetLength(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    const auto *obj = node::ObjectWrap::Unwrap<Dimension>(info.Holder());

    size_t len = 0;
    const int retval = nc_inq_dimlen(obj->parent_id, obj->id, &len);
    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
        return;
    }
    info.GetReturnValue().Set(v8::Integer::New(isolate, static_cast<int32_t>(len)));
}

void Dimension::GetName(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    const auto *obj = node::ObjectWrap::Unwrap<Dimension>(info.Holder());

    std::array<char, NC_MAX_NAME + 1> name{};
    if (obj->get_name(name.data()))
    {
        info.GetReturnValue().Set(
            v8::String::NewFromUtf8(isolate, name.data(), v8::NewStringType::kNormal).ToLocalChecked());
    }
}

void Dimension::SetName(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                        const v8::PropertyCallbackInfo<void> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    auto *obj = node::ObjectWrap::Unwrap<Dimension>(info.Holder());

    const v8::String::Utf8Value new_name_(isolate, val->ToString(isolate->GetCurrentContext()).ToLocalChecked());
    const int retval = nc_rename_dim(obj->parent_id, obj->id, *new_name_);

    if (retval != NC_NOERR)
    {
        throw_netcdf_error(isolate, retval);
    }
}

void Dimension::Inspect(const v8::FunctionCallbackInfo<v8::Value> &args)
{
    v8::Isolate *isolate = args.GetIsolate();
    args.GetReturnValue().Set(
        v8::String::NewFromUtf8(isolate, "[object Dimension]", v8::NewStringType::kNormal).ToLocalChecked());
}
} // namespace netcdf4js
