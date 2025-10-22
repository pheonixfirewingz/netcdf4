#ifndef NODENETCDFJS_VARIABLE_H
#define NODENETCDFJS_VARIABLE_H

#include <array>
#include <netcdf.h>
#include <node.h>
#include <node_object_wrap.h>


namespace nodenetcdfjs
{

/**
 * @brief Represents a NetCDF variable wrapper for Node.js
 * 
 * This class provides a Node.js interface to NetCDF variables, which are the primary
 * data containers in NetCDF files. Variables store multi-dimensional arrays of values
 * along with descriptive attributes. This class supports reading and writing data,
 * configuring compression, chunking, endianness, and other storage properties.
 * 
 * The class extends node::ObjectWrap to provide JavaScript binding capabilities,
 * allowing variables to be manipulated from JavaScript code.
 */
class Variable : public node::ObjectWrap
{
  public:
    /**
     * @brief Initialize the Variable class and register it with Node.js
     * @param exports The exports object to attach the Variable constructor to
     * 
     * This static method sets up the Variable class for use in Node.js,
     * defining its constructor and prototype methods.
     */
    static void Init(v8::Local<v8::Object> exports);
    
    /**
     * @brief Construct a Variable object
     * @param id_ The variable ID in the NetCDF file
     * @param parent_id_ The parent group/file ID
     * 
     * Creates a new variable wrapper for an existing NetCDF variable.
     */
    Variable(int id_, int parent_id_) noexcept;

    /**
     * @brief Get the name of this variable
     * @param name Buffer to store the variable name
     * @return true if successful, false otherwise
     * 
     * Retrieves the variable name from the NetCDF file and stores it
     * in the provided buffer.
     */
    [[nodiscard]] bool get_name(char *name) const noexcept;

  private:
    // Delete copy and move operations for safety
    Variable(const Variable &) = delete;
    Variable &operator=(const Variable &) = delete;
    Variable(Variable &&) = delete;
    Variable &operator=(Variable &&) = delete;

    /**
     * @brief Read the entire variable data
     * @param args JavaScript function arguments
     * 
     * Reads all data from the variable and returns it as a JavaScript array.
     */
    static void Read(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Read a slice of variable data
     * @param args JavaScript function arguments (start indices, counts)
     * 
     * Reads a hyperslab of data from the variable, specified by start positions
     * and count values for each dimension.
     */
    static void ReadSlice(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Read a strided slice of variable data
     * @param args JavaScript function arguments (start indices, counts, strides)
     * 
     * Reads a strided hyperslab of data from the variable, with additional
     * stride parameters to skip elements in each dimension.
     */
    static void ReadStridedSlice(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Write data to the entire variable
     * @param args JavaScript function arguments (data array)
     * 
     * Writes data to all elements of the variable.
     */
    static void Write(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Write data to a slice of the variable
     * @param args JavaScript function arguments (start indices, counts, data)
     * 
     * Writes a hyperslab of data to the variable, specified by start positions
     * and count values for each dimension.
     */
    static void WriteSlice(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Write data to a strided slice of the variable
     * @param args JavaScript function arguments (start indices, counts, strides, data)
     * 
     * Writes a strided hyperslab of data to the variable.
     */
    static void WriteStridedSlice(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Add a new attribute to this variable
     * @param args JavaScript function arguments (name, type, value)
     * 
     * Creates a new attribute associated with this variable.
     */
    static void AddAttribute(const v8::FunctionCallbackInfo<v8::Value> &args);

    /**
     * @brief Getter for the variable ID property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     */
    static void GetId(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the variable type property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns the NetCDF data type of this variable (e.g., "int", "float", "double").
     */
    static void GetType(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the dimensions property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns an array of dimension IDs for this variable.
     */
    static void GetDimensions(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the attributes property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns an object containing all attributes of this variable.
     */
    static void GetAttributes(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the variable name property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     */
    static void GetName(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the variable name property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetName(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                        const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Getter for the endianness property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns the byte order (endianness) of the variable data.
     */
    static void GetEndianness(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the endianness property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetEndianness(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                              const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Getter for the checksum mode property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns the checksum/integrity checking mode for this variable.
     */
    static void GetChecksumMode(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the checksum mode property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetChecksumMode(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                                const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Getter for the chunk mode property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns the chunking mode (contiguous or chunked storage).
     */
    static void GetChunkMode(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the chunk mode property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetChunkMode(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                             const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Getter for the chunk sizes property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns an array of chunk sizes for each dimension.
     */
    static void GetChunkSizes(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the chunk sizes property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetChunkSizes(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                              const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Getter for the fill mode property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns whether fill values are enabled for this variable.
     */
    static void GetFillMode(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the fill mode property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetFillMode(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                            const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Getter for the fill value property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns the fill value used for uninitialized data.
     */
    static void GetFillValue(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the fill value property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetFillValue(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                             const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Getter for the compression shuffle property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns whether byte shuffle filter is enabled (improves compression).
     */
    static void GetCompressionShuffle(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the compression shuffle property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetCompressionShuffle(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                                      const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Getter for the compression deflate property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns whether deflate (gzip) compression is enabled.
     */
    static void GetCompressionDeflate(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the compression deflate property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetCompressionDeflate(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                                      const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Getter for the compression level property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns the compression level (0-9, where 9 is maximum compression).
     */
    static void GetCompressionLevel(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the compression level property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetCompressionLevel(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                                    const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Custom inspect method for Node.js console output
     * @param args JavaScript function arguments
     */
    static void Inspect(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Convert variable to JSON representation
     * @param args JavaScript function arguments
     */
    static void ToJSON(const v8::FunctionCallbackInfo<v8::Value> &args);

    /// Persistent reference to the JavaScript constructor function
    static v8::Persistent<v8::Function> constructor;

    /// Size in bytes for each NetCDF data type
    static constexpr std::array<unsigned char, 11> type_sizes = {1, 1, 2, 4, 4, 8, 1, 2, 4, 8, 0};

    /// String names for each NetCDF data type
    static constexpr std::array<const char *, 11> type_names = {"byte",  "char",   "short", "int",   "float", "double",
                                                                "ubyte", "ushort", "uint",  "int64", "string"};

    /// The variable ID in the NetCDF file
    int id{-1};
    
    /// The parent group/file ID
    int parent_id{-1};
    
    /// The NetCDF data type of this variable
    nc_type type{NC_NAT};
    
    /// Number of dimensions for this variable
    int ndims{0};
};

} // namespace nodenetcdfjs

#endif
