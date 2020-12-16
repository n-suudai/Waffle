#pragma once


#include "hid/peripheral_device.h"
#include "hid/mouse_button.h"
#include "hid/mouse_axis.h"


namespace waffle {
namespace hid {


class IMouse : public IPeripheralDevice
{
public:
    virtual ~IMouse() = default;

    virtual bool isPressed(MouseButton button) const = 0;

    virtual bool isReleased(MouseButton button) const = 0;

    virtual bool isFirstPressed(MouseButton button) const = 0;

    virtual bool isFirstReleased(MouseButton button) const = 0;

    virtual wfl::chrono::microseconds getDurationPressed(MouseButton button) const = 0;

    virtual std::int32_t GetAxisValue(MouseAxis axis) const = 0;
};


} // namespace hid
} // namespace waffle

