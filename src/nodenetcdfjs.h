#ifndef NODENETCDFJS_H
#define NODENETCDFJS_H

#include <netcdf.h>
#include <node.h>
#include <string>
#include <string_view>
#include <unordered_map>

/**
 * @file nodenetcdfjs.h
 * @brief Core utilities and type mappings for the NetCDF4 Node.js binding
 * 
 * This header provides utility functions and type conversion mappings used
 * throughout the NetCDF4 Node.js binding library.
 */

namespace nodenetcdfjs
{

/**
 * @brief Throw a JavaScript exception with a NetCDF error message
 * @param isolate The V8 isolate for the current JavaScript context
 * @param retval The NetCDF error code returned from a NetCDF library function
 * @throws v8::Exception::TypeError with the NetCDF error description
 * 
 * This function converts NetCDF error codes into human-readable error messages
 * and throws them as JavaScript TypeErrors. It uses nc_strerror() to get the
 * error description from the NetCDF library.
 * 
 * @note This function is marked noexcept(false) to indicate it may throw
 */
inline void throw_netcdf_error(v8::Isolate *isolate, int retval) noexcept(false)
{
    isolate->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(isolate, nc_strerror(retval), v8::NewStringType::kNormal).ToLocalChecked()));
}

/**
 * @brief Convert a type name string to a NetCDF type constant
 * @param type_str The string representation of the type (e.g., "int", "float", "double")
 * @return The corresponding NetCDF type constant (e.g., NC_INT, NC_FLOAT, NC_DOUBLE)
 * @retval NC_NAT if the type string is not recognized
 * 
 * This function performs a compile-time mapping from human-readable type names
 * to NetCDF type constants. The following types are supported:
 * - "byte" -> NC_BYTE (signed 8-bit integer)
 * - "char" -> NC_CHAR (8-bit character)
 * - "short" -> NC_SHORT (signed 16-bit integer)
 * - "int" -> NC_INT (signed 32-bit integer)
 * - "float" -> NC_FLOAT (32-bit floating point)
 * - "double" -> NC_DOUBLE (64-bit floating point)
 * - "ubyte" -> NC_UBYTE (unsigned 8-bit integer)
 * - "ushort" -> NC_USHORT (unsigned 16-bit integer)
 * - "uint" -> NC_UINT (unsigned 32-bit integer)
 * - "string" -> NC_STRING (variable-length string)
 * 
 * @note This function is constexpr, allowing it to be evaluated at compile time
 */
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

} // namespace nodenetcdfjs

#endif
