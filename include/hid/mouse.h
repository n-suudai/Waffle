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

    virtual void update(const Duration& deltaTime) = 0;

    virtual bool isAnyPressed() const = 0;

    virtual bool isPressed(MouseButton button) const = 0;

    virtual bool isReleased(MouseButton button) const = 0;

    virtual bool isFirstPressed(MouseButton button) const = 0;

    virtual bool isFirstReleased(MouseButton button) const = 0;

    virtual Duration durationPressed(MouseButton button) const = 0;

    virtual wfl::int32_t axisValue(MouseAxis axis) const = 0;
};


} // namespace hid
} // namespace waffle

