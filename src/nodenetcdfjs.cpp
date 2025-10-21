#include "Attribute.h"
#include "Dimension.h"
#include "File.h"
#include "Group.h"
#include "Variable.h"
#include <node.h>


namespace nodenetcdfjs
{
void InitAll(v8::Local<v8::Object> exports)
{
    File::Init(exports);
    Variable::Init(exports);
    Group::Init(exports);
    Dimension::Init(exports);
    Attribute::Init(exports);
}

NODE_MODULE_INIT()
{
    InitAll(exports);
}
} // namespace nodenetcdfjs
