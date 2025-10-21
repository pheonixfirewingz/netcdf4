#ifndef NETCDF4JS_H
#define NETCDF4JS_H

#include <netcdf.h>
#include <node.h>
#include <string>
#include <string_view>
#include <unordered_map>

namespace netcdf4js
{

inline void throw_netcdf_error(v8::Isolate *isolate, int retval) noexcept(false)
{
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, nc_strerror(retval), v8::NewStringType::kNormal).ToLocalChecked()));
}

[[nodiscard]] constexpr int get_type(std::string_view type_str) noexcept
{
    constexpr std::pair<std::string_view, int> type_map[] = {
        {"byte", NC_BYTE},     {"char", NC_CHAR},   {"short", NC_SHORT},   {"int", NC_INT},   {"float", NC_FLOAT},
        {"double", NC_DOUBLE}, {"ubyte", NC_UBYTE}, {"ushort", NC_USHORT}, {"uint", NC_UINT}, {"string", NC_STRING}};

    for (const auto &[key, value] : type_map)
    {
        if (type_str == key)
        {
            return value;
        }
    }
    return NC_NAT;
}

} // namespace netcdf4js

#endif
