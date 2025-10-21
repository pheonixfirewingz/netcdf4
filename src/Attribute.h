#ifndef NETCDF4JS_ATTRIBUTE_H
#define NETCDF4JS_ATTRIBUTE_H

#include <node.h>
#include <node_object_wrap.h>
#include <string>

namespace netcdf4js {

class Attribute : public node::ObjectWrap {
  public:
    static void Init(v8::Local<v8::Object> exports);
    Attribute(const char* name_, int var_id_, int parent_id_) noexcept;
    Attribute(const char* name_, int var_id_, int parent_id_, int type_) noexcept;
    
    void set_value(const v8::Local<v8::Value>& val);

  private:
    // Delete copy and move operations for safety
    Attribute(const Attribute&) = delete;
    Attribute& operator=(const Attribute&) = delete;
    Attribute(Attribute&&) = delete;
    Attribute& operator=(Attribute&&) = delete;

    static v8::Persistent<v8::Function> constructor;
    
    static void Delete(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void GetName(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetName(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void GetValue(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetValue(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void Inspect(const v8::FunctionCallbackInfo<v8::Value>& args);
    
    std::string name{};
    int var_id{-1};
    int parent_id{-1};
    int type{-1};
};

}  // namespace netcdf4js

#endif
