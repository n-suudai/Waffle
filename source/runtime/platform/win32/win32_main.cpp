

#include "system/common/platform.h"
#include "system/common/types.h"


namespace waffle {
namespace runtime {


extern wfl::int32_t runtimeMain();


} // namespace runtime
} // namespace waffle


int main(int, char*[])
{
    using namespace waffle;
    using namespace runtime;

    wfl::int32_t exitCode = runtimeMain();

    return static_cast<int>(exitCode);
}
