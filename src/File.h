#ifndef NETCDF4JS_FILE_H
#define NETCDF4JS_FILE_H

#include <node.h>
#include <node_object_wrap.h>

namespace netcdf4js
{

class Group;

class File : public node::ObjectWrap
{
  public:
    static void Init(v8::Local<v8::Object> exports);

  private:
    explicit File(int id_) noexcept;
    ~File() override;

    // Delete copy and move operations for safety
    File(const File &) = delete;
    File &operator=(const File &) = delete;
    File(File &&) = delete;
    File &operator=(File &&) = delete;

    [[nodiscard]] bool open(const char *filename, int mode, int format) noexcept;

    static void New(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void Close(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void Sync(const v8::FunctionCallbackInfo<v8::Value> &args);
    static void Inspect(const v8::FunctionCallbackInfo<v8::Value> &args);

    static v8::Persistent<v8::Function> constructor;

    int id{-1};
    bool closed{false};
};

} // namespace netcdf4js

#endif
