﻿#pragma once


#include "modules/hid/peripheral_device.h"
#include "modules/hid/game_pad/game_pad_button.h"
#include "modules/hid/game_pad/analog_input.h"
#include "modules/hid/game_pad/pov.h"


namespace waffle {
namespace hid {


class IGamePad : public IPeripheralDevice
{
public:
    virtual ~IGamePad() = default;

    virtual void update(const Duration& deltaTime) = 0;

    [[nodiscard]] virtual bool isAnyPressed() const = 0;
    [[nodiscard]] virtual bool isAnyButtonPressed() const = 0;
    [[nodiscard]] virtual bool isAnyPOVPressed() const = 0;

    [[nodiscard]] virtual bool isPressed(GamePadButton button) const = 0;
    [[nodiscard]] virtual bool isPressed(POV pov) const = 0;

    [[nodiscard]] virtual bool isReleased(GamePadButton button) const = 0;
    [[nodiscard]] virtual bool isReleased(POV pov) const = 0;

    [[nodiscard]] virtual bool isFirstPressed(GamePadButton button) const = 0;
    [[nodiscard]] virtual bool isFirstPressed(POV pov) const = 0;

    [[nodiscard]] virtual bool isFirstReleased(GamePadButton button) const = 0;
    [[nodiscard]] virtual bool isFirstReleased(POV pov) const = 0;

    [[nodiscard]] virtual Duration durationPressed(GamePadButton button) const = 0;
    [[nodiscard]] virtual Duration durationPressed(POV pov) const = 0;

    [[nodiscard]] virtual wfl::int32_t analogInputValue(AnalogInput analog) const = 0;
};


} // namespace hid
} // namespace waffle

