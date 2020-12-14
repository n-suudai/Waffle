
#include "runtime_heap.h"
#include "core/logging.h"
#include "memory.h"
#include "core.h"
#include "application.h"
#include "hid.h"



namespace waffle {
namespace runtime {


wfl::int32_t runtimeMain()
{
    memory::initialize();
    core::initialize();
    application::initialize();
    hid::initialize();

    logging::format("Hello! %s!", "Waffle");

    {
        constexpr Rectangle<wfl::int32_t> defaultClientRect(0, 640, 0, 480);
        constexpr Rectangle<wfl::int32_t> customClientRect(200, 640, 200, 480);

        UniquePtr<application::IWindow> window = application::createWindowUnique(defaultClientRect);

        window->setTitle("Waffle");
        window->setClientRect(customClientRect);

        while (window->isAlive())
        {
            if (!window->messagePump())
            {
                break;
            }
        }
    }

    hid::finalize();
    application::finalize();
    core::finalize();
    RuntimeHeap::printDebug_Report_MemoryAll(); // メモリの状態を表示
    memory::finalize();

    return 0;
}


} // namespace runtime
} // namespace waffle

