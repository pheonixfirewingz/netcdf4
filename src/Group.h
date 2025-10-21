#ifndef NODENETCDFJS_GROUP_H
#define NODENETCDFJS_GROUP_H

#include <node.h>
#include <node_object_wrap.h>

namespace nodenetcdfjs
{

class Variable;

class Group : public node::ObjectWrap
{
  public:
    explicit Group(int id) noexcept;
    static void Init(v8::Local<v8::Object> exports);

    [[nodiscard]] bool get_name(char *name) const noexcept;

  private:
    // Delete copy and move operations for safety
    Group(const Group &) = delete;
    Group &operator=(const Group &) = delete;
    Group(Group &&) = delete;
    Group &operator=(Group &&) = delete;

    static v8::Persistent<v8::Function> constructor;

    static void GetId(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    static void GetVariables(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    static void GetDimensions(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    static void GetUnlimited(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    static void GetAttributes(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    static void GetSubgroups(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    static void GetName(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    static void GetFullname(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);

    static void AddAttribute(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void AddDimension(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void AddSubgroup(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void AddVariable(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void Inspect(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void ToJSON(const v8::FunctionCallbackInfo<v8::Value> &args);

    int id{-1};
};

} // namespace nodenetcdfjs

#endif
