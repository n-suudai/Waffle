

#include "common/platform/platform.h"
#include "common/types.h"


namespace Engine {
namespace Runtime {


extern EInt32 RuntimeMain();


} // namespace Runtime
} // namespace Engine


int main(int, char*[])
{
    using namespace Engine;
    using namespace Runtime;

    EInt32 exitCode = RuntimeMain();

    return static_cast<int>(exitCode);
}
