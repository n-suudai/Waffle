
#include "runtime_heap.h"
#include "core/logging.h"
#include "memory.h"
#include "core.h"
#include "application.h"



namespace waffle {
namespace runtime {


class Test
{
    WFL_DECLARE_HEAP;
public:
    Test() {}
    ~Test() {}
};
WFL_DEFINE_HEAP(Test, "TEST");


class Runtime
{
    WFL_DECLARE_HEAP;
public:
    Runtime()
    {


        m_test = WFL_MAKE_SHARED(Test);
    }

    ~Runtime()
    {

    }

private:
    memory::SharedPtr<Test> m_test;
};
WFL_DEFINE_HEAP(Runtime, "Runtime");




wfl::int32_t runtimeMain()
{
    memory::initialize();
    core::initialize();
    application::initialize();

    logging::format("Hello! %s!", "Waffle");

    {
        constexpr Rectangle<wfl::uint32_t> defaultClientRect(0, 640, 0, 480);

        memory::UniquePtr<application::IWindow> window = application::createWindowUnique(defaultClientRect);
    }

    application::finalize();
    core::finalize();
    RuntimeHeap::printDebug_Report_MemoryAll(); // メモリの状態を表示
    memory::finalize();

    return 0;
}


} // namespace runtime
} // namespace waffle

