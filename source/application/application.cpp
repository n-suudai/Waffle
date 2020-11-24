#include "application.h"
#include "core/logging.h"



namespace waffle {
namespace application {


static Allocation s_allocation;


bool initialize(Allocation allocation)
{
    s_allocation = allocation;

    logging::put("[application] initialized.");

    return true;
}

void finalize()
{
    logging::put("[application] finalized.");
}


} // namespace application
} // namespace waffle
