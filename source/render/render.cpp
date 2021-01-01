
#include "render.h"
#include "core/logging.h"


namespace waffle {
namespace render {


bool initialize()
{
    logging::put("[render] initialized.");

    return true;
}

void finalize()
{
    logging::put("[render] finalized.");
}


} // namespace render
} // namespace waffle



