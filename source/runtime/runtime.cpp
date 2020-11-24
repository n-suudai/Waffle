
#include "core.h"
#include "application.h"


namespace waffle {
namespace runtime {


wfl::int32_t runtimeMain()
{
    core::initialize();
    application::initialize();

    application::finalize();
    core::finalize();

    return 0;
}


} // namespace runtime
} // namespace waffle

