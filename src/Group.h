#ifndef NODENETCDFJS_GROUP_H
#define NODENETCDFJS_GROUP_H

#include <node.h>
#include <node_object_wrap.h>

namespace nodenetcdfjs
{

class Variable;

/**
 * @brief Represents a NetCDF group wrapper for Node.js
 * 
 * This class provides a Node.js interface to NetCDF groups, which are hierarchical
 * containers for organizing variables, dimensions, attributes, and subgroups within
 * a NetCDF file. Groups enable logical organization of related data items.
 * 
 * The class extends node::ObjectWrap to provide JavaScript binding capabilities,
 * allowing groups to be navigated and manipulated from JavaScript code.
 */
class Group : public node::ObjectWrap
{
  public:
    /**
     * @brief Construct a Group object
     * @param id The group ID in the NetCDF file
     * 
     * Creates a new group wrapper for an existing NetCDF group.
     */
    explicit Group(int id) noexcept;
    
    /**
     * @brief Initialize the Group class and register it with Node.js
     * @param exports The exports object to attach the Group constructor to
     * 
     * This static method sets up the Group class for use in Node.js,
     * defining its constructor and prototype methods.
     */
    static void Init(v8::Local<v8::Object> exports);

    /**
     * @brief Get the name of this group
     * @param name Buffer to store the group name
     * @return true if successful, false otherwise
     * 
     * Retrieves the group name from the NetCDF file and stores it
     * in the provided buffer.
     */
    [[nodiscard]] bool get_name(char *name) const noexcept;

  private:
    // Delete copy and move operations for safety
    Group(const Group &) = delete;
    Group &operator=(const Group &) = delete;
    Group(Group &&) = delete;
    Group &operator=(Group &&) = delete;

    /// Persistent reference to the JavaScript constructor function
    static v8::Persistent<v8::Function> constructor;

    /**
     * @brief Getter for the group ID property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     */
    static void GetId(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the variables property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns an object containing all variables in this group.
     */
    static void GetVariables(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the dimensions property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns an object containing all dimensions in this group.
     */
    static void GetDimensions(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the unlimited dimensions property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns an array of unlimited dimension IDs in this group.
     */
    static void GetUnlimited(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the attributes property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns an object containing all attributes in this group.
     */
    static void GetAttributes(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the subgroups property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns an object containing all subgroups in this group.
     */
    static void GetSubgroups(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the group name property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     */
    static void GetName(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);
    
    /**
     * @brief Getter for the full group name property
     * @param property The property name being accessed
     * @param info Callback info containing the return value
     * 
     * Returns the full path to this group from the root.
     */
    static void GetFullname(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info);

    /**
     * @brief Add a new attribute to this group
     * @param args JavaScript function arguments (name, type, value)
     * 
     * Creates a new attribute with the specified name, type, and value.
     */
    static void AddAttribute(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Add a new dimension to this group
     * @param args JavaScript function arguments (name, length)
     * 
     * Creates a new dimension with the specified name and length.
     * Use 0 or NC_UNLIMITED for an unlimited dimension.
     */
    static void AddDimension(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Add a new subgroup to this group
     * @param args JavaScript function arguments (name)
     * 
     * Creates a new subgroup with the specified name.
     */
    static void AddSubgroup(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Add a new variable to this group
     * @param args JavaScript function arguments (name, type, dimensions)
     * 
     * Creates a new variable with the specified name, data type, and dimensions.
     */
    static void AddVariable(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Custom inspect method for Node.js console output
     * @param args JavaScript function arguments
     */
    static void Inspect(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Convert group to JSON representation
     * @param args JavaScript function arguments
     */
    static void ToJSON(const v8::FunctionCallbackInfo<v8::Value> &args);

    /// The group ID in the NetCDF file
    int id{-1};
};

} // namespace nodenetcdfjs

#endif
