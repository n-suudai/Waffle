#pragma once


#include "hid/peripheral_device.h"
#include "hid/game_pad_button.h"
#include "hid/analog_input.h"
#include "hid/pov.h"


namespace waffle {
namespace hid {


class IGamePad : public IPeripheralDevice
{
public:
    virtual ~IGamePad() = default;

    virtual bool isPressed(GamePadButton button) const = 0;
    virtual bool isPressed(POV pov) const = 0;

    virtual bool isReleased(GamePadButton button) const = 0;
    virtual bool isReleased(POV pov) const = 0;

    virtual bool isFirstPressed(GamePadButton button) const = 0;
    virtual bool isFirstPressed(POV pov) const = 0;

    virtual bool isFirstReleased(GamePadButton button) const = 0;
    virtual bool isFirstReleased(POV pov) const = 0;

    virtual wfl::chrono::microseconds getDurationPressed(GamePadButton button) const = 0;
    virtual wfl::chrono::microseconds getDurationPressed(POV pov) const = 0;

    virtual wfl::int32_t AnalogInputValue(AnalogInput analog) const = 0;
};


} // namespace hid
} // namespace waffle

