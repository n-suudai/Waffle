#pragma once


#include "system/memory/memory.h"


namespace waffle {
namespace render {


class RenderEntity
{
    WFL_DECLARE_HEAP;
protected:
    RenderEntity() = default;
    virtual ~RenderEntity() = default;
};


} // namespace render
} // namespace waffle

