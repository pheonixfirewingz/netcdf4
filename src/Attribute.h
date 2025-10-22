#ifndef NODENETCDFJS_ATTRIBUTE_H
#define NODENETCDFJS_ATTRIBUTE_H

#include <node.h>
#include <node_object_wrap.h>
#include <string>

namespace nodenetcdfjs
{

/**
 * @brief Represents a NetCDF attribute wrapper for Node.js
 * 
 * This class provides a Node.js interface to NetCDF attributes, which are metadata
 * associated with variables or groups. Attributes can store information such as
 * units, descriptions, valid ranges, and other metadata about the data.
 * 
 * The class extends node::ObjectWrap to provide JavaScript binding capabilities,
 * allowing attributes to be manipulated from JavaScript code.
 */
class Attribute : public node::ObjectWrap
{
  public:
    /**
     * @brief Initialize the Attribute class and register it with Node.js
     * @param exports The exports object to attach the Attribute constructor to
     * 
     * This static method sets up the Attribute class for use in Node.js,
     * defining its constructor and prototype methods.
     */
    static void Init(v8::Local<v8::Object> exports);
    
    /**
     * @brief Construct an Attribute object
     * @param name_ The name of the attribute
     * @param var_id_ The variable ID this attribute belongs to
     * @param parent_id_ The parent group/file ID
     * 
     * Creates a new attribute with the specified name and parent identifiers.
     */
    Attribute(const char *name_, int var_id_, int parent_id_) noexcept;
    
    /**
     * @brief Construct an Attribute object with explicit type
     * @param name_ The name of the attribute
     * @param var_id_ The variable ID this attribute belongs to
     * @param parent_id_ The parent group/file ID
     * @param type_ The NetCDF data type of the attribute
     * 
     * Creates a new attribute with the specified name, parent identifiers, and data type.
     */
    Attribute(const char *name_, int var_id_, int parent_id_, int type_) noexcept;

    /**
     * @brief Set the value of the attribute
     * @param val The value to set (as a V8 value from JavaScript)
     * 
     * Sets the attribute's value from a JavaScript value, performing appropriate
     * type conversions as needed.
     */
    void set_value(const v8::Local<v8::Value> &val);

  private:
    // Delete copy and move operations for safety
    Attribute(const Attribute &) = delete;
    Attribute &operator=(const Attribute &) = delete;
    Attribute(Attribute &&) = delete;
    Attribute &operator=(Attribute &&) = delete;

    /// Persistent reference to the JavaScript constructor function
    static v8::Persistent<v8::Function> constructor;

    /**
     * @brief Delete this attribute from the NetCDF file
     * @param args JavaScript function arguments
     */
    static void Delete(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Getter for the attribute name property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     */
    static void GetName(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the attribute name property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetName(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                        const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Getter for the attribute value property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     */
    static void GetValue(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Setter for the attribute value property
     * @param property The property name being set
     * @param val The new value to set
     * @param info Callback info for the setter
     */
    static void SetValue(v8::Local<v8::String> property, v8::Local<v8::Value> val,
                         const v8::PropertyCallbackInfo<void> &info);
    
    /**
     * @brief Custom inspect method for Node.js console output
     * @param args JavaScript function arguments
     */
    static void Inspect(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Convert attribute to JSON representation
     * @param args JavaScript function arguments
     */
    static void ToJSON(const v8::FunctionCallbackInfo<v8::Value> &args);

    /// The name of the attribute
    std::string name{};
    
    /// The variable ID this attribute belongs to (-1 for global attributes)
    int var_id{-1};
    
    /// The parent group/file ID
    int parent_id{-1};
    
    /// The NetCDF data type of the attribute
    int type{-1};
};

} // namespace nodenetcdfjs

#endif
