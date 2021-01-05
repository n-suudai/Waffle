#pragma once


#ifndef DIRECTINPUT_VERSION

#define DIRECTINPUT_VERSION  0x0800

#endif

#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#pragma warning(push)
#pragma warning(disable:6000 28251)
#include <dinput.h>
#pragma warning(pop)

#include <dinputd.h>

#include "common/platform/win32.h"

