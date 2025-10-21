#ifndef NETCDF4JS_VARIABLE_H
#define NETCDF4JS_VARIABLE_H

#include <netcdf.h>
#include <node.h>
#include <node_object_wrap.h>
#include <array>

namespace netcdf4js {

class Variable : public node::ObjectWrap {
  public:
    static void Init(v8::Local<v8::Object> exports);
    Variable(int id_, int parent_id_) noexcept;
    
    [[nodiscard]] bool get_name(char* name) const noexcept;

  private:
    // Delete copy and move operations for safety
    Variable(const Variable&) = delete;
    Variable& operator=(const Variable&) = delete;
    Variable(Variable&&) = delete;
    Variable& operator=(Variable&&) = delete;

    static void Read(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void ReadSlice(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void ReadStridedSlice(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Write(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void WriteSlice(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void WriteStridedSlice(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void AddAttribute(const v8::FunctionCallbackInfo<v8::Value>& args);
    
    static void GetId(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetType(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetDimensions(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetAttributes(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void GetName(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetName(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void GetEndianness(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetEndianness(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void GetChecksumMode(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetChecksumMode(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void GetChunkMode(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetChunkMode(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void GetChunkSizes(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetChunkSizes(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void GetFillMode(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetFillMode(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void GetFillValue(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetFillValue(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void GetCompressionShuffle(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetCompressionShuffle(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void GetCompressionDeflate(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetCompressionDeflate(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void GetCompressionLevel(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetCompressionLevel(v8::Local<v8::String> property, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info);
    static void Inspect(const v8::FunctionCallbackInfo<v8::Value>& args);
    
    static v8::Persistent<v8::Function> constructor;
    
    // Use constexpr arrays for compile-time constants
    static constexpr std::array<unsigned char, 11> type_sizes = {
        1, 1, 2, 4, 4, 8, 1, 2, 4, 8, 0
    };
    
    static constexpr std::array<const char*, 11> type_names = {
        "byte", "char", "short", "int", "float", "double",
        "ubyte", "ushort", "uint", "int64", "string"
    };
    
    int id{-1};
    int parent_id{-1};
    nc_type type{NC_NAT};
    int ndims{0};
};

}  // namespace netcdf4js

#endif
