#pragma once


#include "hid/entity.h"
#include <chrono>


namespace waffle {
namespace hid {


class IPeripheralDeviceManager : public HIDEntity
{
public:
    virtual ~IPeripheralDeviceManager() = default;

    virtual void updateAll(wfl::chrono::microseconds deltaTime) = 0;

    virtual wfl::size_t getKeyboardCount() const = 0;

    virtual wfl::size_t getMouseCount() const = 0;
    
    virtual wfl::size_t getGamePadCount() const = 0;
};


} // namespace hid
} // namespace waffle

