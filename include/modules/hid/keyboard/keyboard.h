#pragma once


#include "modules/hid/peripheral_device.h"
#include "modules/hid/keyboard/key_code.h"


namespace waffle {
namespace hid {


class IKeyboard : public IPeripheralDevice
{
public:
    virtual ~IKeyboard() = default;

    virtual void update(const Duration& deltaTime) = 0;

    virtual bool isAnyPressed() const = 0;

    virtual bool isPressed(KeyCode key) const = 0;

    virtual bool isReleased(KeyCode key) const = 0;

    virtual bool isFirstPressed(KeyCode key) const = 0;

    virtual bool isFirstReleased(KeyCode key) const = 0;

    virtual Duration durationPressed(KeyCode key) const = 0;
};


} // namespace hid
} // namespace waffle

