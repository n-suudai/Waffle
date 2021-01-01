
#include "memory.h"
#include "core.h"
#include "application.h"
#include "hid.h"
#include "render.h"

#include "core/logging.h"

#include "runtime_heap.h"
#include "common/utility/loop_timer.h"
#include "game_input.h"


namespace waffle {
namespace runtime {


bool runtimeInitialize()
{
    if (!memory::initialize()) { return false; }
    if (!core::initialize()) { return false; }
    if (!application::initialize()) { return false; }
    if (!hid::initialize()) { return false; }
    if (!render::initialize()) { return false; }
    return true;
}

void runtimeFinalize()
{
    render::finalize();
    hid::finalize();
    application::finalize();
    core::finalize();
    RuntimeHeap::printDebug_Report_MemoryAll(); // メモリの状態を表示
    memory::finalize();
}

void runtimeBody()
{
    logging::format("Hello! %s!", "Waffle");

    constexpr Rectangle<wfl::int32_t> defaultClientRect(0, 640, 0, 480);
    constexpr Rectangle<wfl::int32_t> customClientRect(200, 640, 200, 480);

    UniquePtr<application::IWindow> window;
    if (!application::createWindowUnique(defaultClientRect, window)) { return; }
    if (!window->setTitle("Waffle")) { return; }
    if (!window->setClientRect(customClientRect)) { return; }

    LoopTimer loopTimer;
    GameInput gameInput;

    if (!gameInput.initialize(window)) { return; }

    // loop
    while (window->isAlive())
    {
        hid::Duration elapsed = loopTimer.update<hid::Duration>();

        gameInput.update(elapsed);

        if (gameInput.isAnyPressed())
        {
            break;
        }

        if (!window->messagePump())
        {
            break;
        }
    }
}


wfl::int32_t runtimeMain()
{
    if (!runtimeInitialize())
    {
        runtimeFinalize();
        return 0;
    }

    runtimeBody();

    runtimeFinalize();

    return 0;
}


} // namespace runtime
} // namespace waffle

