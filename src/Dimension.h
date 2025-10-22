#ifndef NODENETCDFJS_DIMENSION_H
#define NODENETCDFJS_DIMENSION_H

#include <node.h>
#include <node_object_wrap.h>

namespace nodenetcdfjs
{

/**
 * @brief Represents a NetCDF dimension wrapper for Node.js
 * 
 * This class provides a Node.js interface to NetCDF dimensions, which define
 * the shape and size of variables in a NetCDF file. Dimensions can be either
 * fixed-length or unlimited (allowing growth over time).
 * 
 * The class extends node::ObjectWrap to provide JavaScript binding capabilities,
 * allowing dimensions to be queried and manipulated from JavaScript code.
 */
class Dimension : public node::ObjectWrap
{
  public:
    /**
     * @brief Initialize the Dimension class and register it with Node.js
     * @param exports The exports object to attach the Dimension constructor to
     * 
     * This static method sets up the Dimension class for use in Node.js,
     * defining its constructor and prototype methods.
     */
    static void Init(v8::Local<v8::Object> exports);
    
    /**
     * @brief Construct a Dimension object
     * @param id_ The dimension ID in the NetCDF file
     * @param parent_id_ The parent group/file ID
     * 
     * Creates a new dimension wrapper for an existing NetCDF dimension.
     */
    Dimension(int id_, int parent_id_) noexcept;

    /**
     * @brief Get the name of this dimension
     * @param name Buffer to store the dimension name
     * @return true if successful, false otherwise
     * 
     * Retrieves the dimension name from the NetCDF file and stores it
     * in the provided buffer.
     */
    [[nodiscard]] bool get_name(char *name) const noexcept;

  private:
    // Delete copy and move operations for safety
    Dimension(const Dimension &) = delete;
    Dimension &operator=(const Dimension &) = delete;
    Dimension(Dimension &&) = delete;
    Dimension &operator=(Dimension &&) = delete;

    /// Persistent reference to the JavaScript constructor function
    static v8::Persistent<v8::Function> constructor;

    /**
     * @brief Getter for the dimension ID property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     */
    static void GetId(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the dimension length property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     */
    static void GetLength(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the dimension name property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     */
    static void GetName(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the dimension name property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetName(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                        const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Custom inspect method for Node.js console output
     * @param args JavaScript function arguments
     */
    static void Inspect(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Convert dimension to JSON representation
     * @param args JavaScript function arguments
     */
    static void ToJSON(const v8::FunctionCallbackInfo<v8::Value> &args);

    /// The dimension ID in the NetCDF file
    int id{-1};
    
    /// The parent group/file ID
    int parent_id{-1};
};

} // namespace nodenetcdfjs

#endif
