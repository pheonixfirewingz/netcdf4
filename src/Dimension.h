#ifndef NETCDF4JS_DIMENSION_H
#define NETCDF4JS_DIMENSION_H

#include <node.h>
#include <node_object_wrap.h>

namespace netcdf4js {

class Dimension : public node::ObjectWrap {
  public:
    static void Init(v8::Local<v8::Object> exports);
    Dimension(int id_, int parent_id_) noexcept;
    
    [[nodiscard]] bool get_name(char* name) const noexcept;

  private:
    // Delete copy and move operations for safety
    Dimension(const Dimension&) = delete;
    Dimension& operator=(const Dimension&) = delete;
    Dimension(Dimension&&) = delete;
    Dimension& operator=(Dimension&&) = delete;

    static v8::Persistent<v8::Function> constructor;
    
    static void GetId(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetLength(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetName(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetName(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void Inspect(const v8::FunctionCallbackInfo<v8::Value>& args);
    
    int id{-1};
    int parent_id{-1};
};

}  // namespace netcdf4js

#endif
