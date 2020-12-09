
#include "runtime_heap.h"
#include "core/logging.h"
#include "memory.h"
#include "core.h"
#include "application.h"



namespace waffle {
namespace runtime {


wfl::int32_t runtimeMain()
{
    memory::initialize();
    core::initialize();
    application::initialize();

    logging::format("Hello! %s!", "Waffle");

    {
        constexpr Rectangle<wfl::int32_t> defaultClientRect(0, 640, 0, 480);

        memory::UniquePtr<application::IWindow> window = application::createWindowUnique(defaultClientRect);

        while (window->isAlive())
        {
            if (!window->messagePump())
            {
                break;
            }
        }
    }

    application::finalize();
    core::finalize();
    RuntimeHeap::printDebug_Report_MemoryAll(); // メモリの状態を表示
    memory::finalize();

    return 0;
}


} // namespace runtime
} // namespace waffle

