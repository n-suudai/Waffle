#pragma once


#include "system/common/types.h"


#ifndef WFL_NO_NEW_DELETE


// new / delete 不可
#define WFL_NO_NEW_DELETE \
static void* operator new(waffle::wfl::size_t) = delete; \
static void* operator new[](waffle::wfl::size_t) = delete; \
static void operator delete(void*) = delete; \
static void operator delete[](void*) = delete; \


#endif // WFL_NO_NEW_DELETE

