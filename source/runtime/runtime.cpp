
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
        memory::initialize();
        core::initialize();
        application::initialize();

        m_test = WFL_MAKE_SHARED(Test);
    }

    ~Runtime()
    {
        application::finalize();
        core::finalize();

        m_test.reset();

        RuntimeHeap::printDebug_Report_MemoryAll(); // メモリの状態を表示
        memory::finalize();
    }

private:
    memory::SharedPtr<Test> m_test;
};
WFL_DEFINE_HEAP(Runtime, "Runtime");




wfl::int32_t runtimeMain()
{
    Runtime runtime;

    logging::format("%s ...", "Waffle");

    return 0;
}


} // namespace runtime
} // namespace waffle

