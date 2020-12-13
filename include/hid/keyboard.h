#pragma once


#include "hid/peripheral_device.h"
#include "hid/key_code.h"


namespace waffle {
namespace hid {


class IKeyBoard : public IPeripheralDevice
{
public:
    virtual ~IKeyBoard() = default;

    virtual bool isPressed(KeyCode key) const = 0;

    virtual bool isReleased(KeyCode key) const = 0;

    virtual bool isFirstPressed(KeyCode key) const = 0;

    virtual bool isFirstReleased(KeyCode key) const = 0;

    virtual wfl::chrono::microseconds getDurationPressed(KeyCode key) const = 0;
};


} // namespace hid
} // namespace waffle

