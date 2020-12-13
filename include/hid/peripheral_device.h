#pragma once


#include "hid/entity.h"
#include <chrono>


namespace waffle {
namespace hid {


class IPeripheralDevice : public HIDEntity
{
public:
    virtual ~IPeripheralDevice() = default;

    virtual void update(wfl::chrono::microseconds deltaTime) = 0;

    virtual bool isAnyPressed() const = 0;
};


} // namespace hid
} // namespace waffle

