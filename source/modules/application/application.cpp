#include "modules/application/application.h"



namespace waffle {
namespace application {


bool initialize()
{
    logging::put("[application] initialized.");

    return true;
}

void finalize()
{
    logging::put("[application] finalized.");
}


} // namespace application
} // namespace waffle
