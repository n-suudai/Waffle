#pragma once


#include "modules/hid/peripheral_device.h"
#include "modules/hid/mouse/mouse_button.h"
#include "modules/hid/mouse/mouse_axis.h"


namespace waffle {
namespace hid {


class IMouse : public IPeripheralDevice
{
public:
    virtual ~IMouse() = default;

    virtual void update(const Duration& deltaTime) = 0;

    [[nodiscard]] virtual bool isAnyPressed() const = 0;

    [[nodiscard]] virtual bool isPressed(MouseButton button) const = 0;

    [[nodiscard]] virtual bool isReleased(MouseButton button) const = 0;

    [[nodiscard]] virtual bool isFirstPressed(MouseButton button) const = 0;

    [[nodiscard]] virtual bool isFirstReleased(MouseButton button) const = 0;

    [[nodiscard]] virtual Duration durationPressed(MouseButton button) const = 0;

    [[nodiscard]] virtual wfl::int32_t axisValue(MouseAxis axis) const = 0;
};


} // namespace hid
} // namespace waffle

