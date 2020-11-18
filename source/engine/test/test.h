#pragma once


#include "common/types.h"


#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)

#ifdef DLL_BUILD
#define DLL_API EXPORT
#else
#define DLL_API IMPORT
#endif


extern "C" {


    EBool DLL_API Initialize_Test();

    EVoid DLL_API Finalize_Test();


} // extern "C"

