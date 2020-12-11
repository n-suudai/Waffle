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



} // namespace hid
} // namespace waffle

