#pragma once


#include "memory.h"


namespace waffle {
namespace hid {


class HIDEntity
{
    WFL_DECLARE_HEAP;
protected:
    HIDEntity() = default;
    virtual ~HIDEntity() = default;
};


typedef wfl::chrono::microseconds Duration;
constexpr Duration DURATION_ZERO = Duration(0);


} // namespace hid
} // namespace waffle

