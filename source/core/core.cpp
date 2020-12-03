#include "core.h"
#include "core/logging.h"


namespace waffle {
namespace core {


bool initialize()
{
    logging::put("[core] initialized.");

    return true;
}

void finalize()
{
    logging::put("[core] finalized.");
}


} // namespace core
} // namespace waffle
