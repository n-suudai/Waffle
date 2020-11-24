#include "core.h"
#include "memory/memory.h"
#include "core/logging.h"


namespace waffle {
namespace core {


bool initialize()
{
    bool result = memory::initialize();

    logging::put("[core] initialized.");

    return result;
}

void finalize()
{
    logging::put("[core] finalized.");
}


} // namespace core
} // namespace waffle
