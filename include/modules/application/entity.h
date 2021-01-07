#pragma once


#include "system/memory/memory.h"


namespace waffle {
namespace application {


class ApplicationEntity
{
    WFL_DECLARE_HEAP;
protected:
    ApplicationEntity() = default;
    virtual ~ApplicationEntity() = default;
};


} // namespace application
} // namespace waffle

