#include "hid/hid.h"
#include "core/logging/logging.h"


namespace Engine {
namespace HID {


EBool Initialize()
{
    Core::OutputLog("[HID] Initialize");
    return true;
}

EVoid Finalize()
{
    Core::OutputLog("[HID] Finalize");
}


} // namespace HID
} // namespace Engine

