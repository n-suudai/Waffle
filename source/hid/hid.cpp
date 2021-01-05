#include "hid.h"


namespace waffle {
namespace hid {


bool initialize()
{
    logging::put("[hid] initialized.");

    return true;
}

void finalize()
{
    logging::put("[hid] finalized.");
}


} // namespace hid
} // namespace waffle


