#include <cassert>
#include "memory.h"
#include "memory/alloc_header.h"

#include "memory/heap_walk.h"
#include "memory/heap_factory.h"


#include "common/platform/win32.h"


class TestClass
{
    WFL_DECLARE_HEAP;
public:
    TestClass() {}
    ~TestClass() {}

private:

};

struct TestStruct
{
    int a;
};



#define DebugPrintf(format, ...)              \
    {                                         \
        char buff[256];                       \
        sprintf_s(buff, format, __VA_ARGS__); \
        ::OutputDebugStringA(buff);             \
    }


namespace waffle {
namespace memory {


void printAllocInfo(const Heap* pHeap, const AllocHeader* pAllocHeader)
{
    assert(pHeap != nullptr);
    assert(pAllocHeader != nullptr);

    DebugPrintf(
        "%s(%d)\n"
        "{ heap=\"%s\" address=0x%p size=%zubyte time=%s "
        "backTraceHash=0x%016zX bookmark=%zX }\n"
        "SIGNATURE=[ %08X ]\n",
        pAllocHeader->getFileName(), pAllocHeader->getLine(), pHeap->getName(),
        pAllocHeader->getBlock(), pAllocHeader->getBytes(),
        /*pAllocation->time*/ "", pAllocHeader->getBackTraceHash(),
        pAllocHeader->getBookmark(), pAllocHeader->getSignature());
}

class TreeStatsReporter : public IHeapTreeStatsReporter
{
public:
    void beginReport() override
    {
        DebugPrintf("MEMORY INFORMATION\n");
        DebugPrintf(
            "                            Local                 Total\n");
        DebugPrintf(
            "Name                  Memory  Peak  Inst   Memory  Peak  Inst\n");
    }

    void report(
        wfl::int32_t depth,
        const Heap* pHeap,
        const HeapTreeStats& local,
        const HeapTreeStats& total) override
    {
        for (int i = 0; i < depth; ++i)
        {
            DebugPrintf("..");
        }

        int spacing = 20 - depth * 2;
        DebugPrintf("%-*s %6zu %6zu %5zu  %6zu %6zu %5zu\n", spacing,
            pHeap->getName(), local.totalBytes, local.peakBytes,
            local.instanceCount, total.totalBytes, total.peakBytes,
            total.instanceCount);
    }

    void endReport() override
    {}
};

class LeakReporter : public IMemoryLeakReporter
{
public:
    LeakReporter() : m_leakCount(0)
    {}

    void beginReport() override
    {
        m_leakCount = 0;
    }

    void report(const Heap* pHeap, const AllocHeader* pAllocHeader) override
    {
        printAllocInfo(pHeap, pAllocHeader);
        m_leakCount++;
    }

    void endReport() override
    {
        if (m_leakCount > 0)
        {
            DebugPrintf("%zu memory leaks found!\n", m_leakCount);
        }
        else
        {
            DebugPrintf("No memory leaks detected.\n");
        }
    }

public:
    size_t m_leakCount;
};

class AssertionReporter : public IMemoryAssertionReporter
{
public:
    AssertionReporter() : m_assertionCount(0)
    {}

    void beginReport() override
    {
        m_assertionCount = 0;
    }

    void report(const Heap* pHeap, const AllocHeader* pAllocHeader) override
    {
        printAllocInfo(pHeap, pAllocHeader);
        m_assertionCount++;
    }

    void endReport() override
    {
        if (m_assertionCount > 0)
        {
            DebugPrintf("%zu memory assertions occurred!\n", m_assertionCount);
        }
        else
        {
            DebugPrintf("No memory assertions detected.\n");
        }
    }

public:
    size_t m_assertionCount;
};

void PrintDebug_Report_HeapTreeStats()
{
    TreeStatsReporter statsReporter;
    HeapFactory::get().reportHeapTreeStats(&statsReporter);
}
void PrintDebug_Report_MemoryLeaks()
{
    LeakReporter leakReporter;
    HeapFactory::get().reportMemoryLeaks(&leakReporter);
}
void PrintDebug_Report_MemoryAssertions()
{
    AssertionReporter assertionReporter;
    HeapFactory::get().reportMemoryAssertions(&assertionReporter);
}
void PrintDebug_Report_MemoryAll()
{
    PrintDebug_Report_HeapTreeStats();
    PrintDebug_Report_MemoryLeaks();
    PrintDebug_Report_MemoryAssertions();
}

bool initialize()
{
    HeaderInfoFlags flags = HeaderInfoFlagBits::All;
    bool result =  AllocHeader::initialize(flags);

    {
        SharedPtr<TestStruct> p = WFL_GLOBAL_HEAP_MAKE_SHARED(TestStruct);
        PrintDebug_Report_MemoryAll();
    }


    PrintDebug_Report_MemoryAll();


    //PrintDebug_Report_MemoryAll();

    return result;
}

void finalize()
{

}


} // namespace memory
} // namespace waffle

