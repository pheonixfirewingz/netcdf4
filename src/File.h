#ifndef NODENETCDFJS_FILE_H
#define NODENETCDFJS_FILE_H

#include <node.h>
#include <node_object_wrap.h>

namespace nodenetcdfjs
{

class Group;

/**
 * @brief Represents a NetCDF file wrapper for Node.js
 * 
 * This class provides a Node.js interface to NetCDF files, which are used for
 * storing array-oriented scientific data. The File class handles opening, closing,
 * and synchronizing NetCDF files, and serves as the entry point for accessing
 * groups, variables, dimensions, and attributes within the file.
 * 
 * The class extends node::ObjectWrap to provide JavaScript binding capabilities,
 * allowing NetCDF files to be manipulated from JavaScript code.
 */
class File : public node::ObjectWrap
{
  public:
    /**
     * @brief Initialize the File class and register it with Node.js
     * @param exports The exports object to attach the File constructor to
     * 
     * This static method sets up the File class for use in Node.js,
     * defining its constructor and prototype methods.
     */
    static void Init(v8::Local<v8::Object> exports);

  private:
    /**
     * @brief Construct a File object
     * @param id_ The file ID from the NetCDF library
     * 
     * Creates a new file wrapper for an opened NetCDF file.
     */
    explicit File(int id_) noexcept;
    
    /**
     * @brief Destructor for File object
     * 
     * Ensures the NetCDF file is properly closed when the object is destroyed.
     */
    ~File() override;

    // Delete copy and move operations for safety
    File(const File &) = delete;
    File &operator=(const File &) = delete;
    File(File &&) = delete;
    File &operator=(File &&) = delete;

    /**
     * @brief Open a NetCDF file
     * @param filename Path to the NetCDF file to open
     * @param mode Access mode (read-only, read-write, create, etc.)
     * @param format NetCDF format (classic, 64-bit offset, NetCDF-4, etc.)
     * @return true if successful, false otherwise
     * 
     * Opens a NetCDF file with the specified mode and format.
     */
    [[nodiscard]] bool open(const char *filename, int mode, int format) noexcept;

    /**
     * @brief JavaScript constructor for creating new File objects
     * @param args JavaScript function arguments containing filename, mode, and format
     */
    static void New(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Close the NetCDF file
     * @param args JavaScript function arguments
     * 
     * Closes the file and releases associated resources.
     */
    static void Close(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Synchronize file to disk
     * @param args JavaScript function arguments
     * 
     * Flushes any buffered data to disk, ensuring data persistence.
     */
    static void Sync(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Custom inspect method for Node.js console output
     * @param args JavaScript function arguments
     */
    static void Inspect(const v8::FunctionCallbackInfo<v8::Value> &args);
    
    /**
     * @brief Convert file to JSON representation
     * @param args JavaScript function arguments
     */
    static void ToJSON(const v8::FunctionCallbackInfo<v8::Value> &args);

    /// Persistent reference to the JavaScript constructor function
    static v8::Persistent<v8::Function> constructor;

    /// The file ID from the NetCDF library
    int id{-1};
    
    /// Flag indicating whether the file has been closed
    bool closed{false};
};

} // namespace nodenetcdfjs

#endif
