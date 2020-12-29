﻿#pragma once


#include "memory.h"


namespace waffle {


class RuntimeEntity
{
    WFL_DECLARE_HEAP;
protected:
    RuntimeEntity() = default;
    virtual ~RuntimeEntity() = default;
};


} // namespace waffle

