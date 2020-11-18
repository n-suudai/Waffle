#include "test/test.h"
#include "core/logging/logging.h"


extern "C" {


    EBool DLL_API Initialize_Test()
    {
        Engine::Core::OutputLog("[Test] Initialize");

        return true;
    }

    EVoid DLL_API Finalize_Test()
    {
        Engine::Core::OutputLog("[Test] Finalize");
    }


} // extern "C"

