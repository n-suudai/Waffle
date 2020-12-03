
#include "memory.h"
#include "core.h"
#include "application.h"


namespace waffle {
namespace runtime {


wfl::int32_t runtimeMain()
{
    memory::initialize();
    core::initialize();
    application::initialize();

    application::finalize();
    core::finalize();
    memory::finalize();

    return 0;
}


} // namespace runtime
} // namespace waffle

