#pragma once


#include "memory.h"


namespace waffle {
namespace modules {


class ModulesEntity
{
    WFL_DECLARE_HEAP;
protected:
    ModulesEntity() = default;
    virtual ~ModulesEntity() = default;
};


} // namespace modules
} // namespace waffle

