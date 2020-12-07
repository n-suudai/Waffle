
#include "runtime_heap.h"
#include "memory/heap_walk.h"
#include "memory/heap_factory.h"
#include "core/logging.h"
#include <cassert>


namespace waffle {
namespace runtime {


static void printAllocInfo(
    const memory::Heap* pHeap,
    const memory::AllocHeader* pAllocHeader)
{
    assert(pHeap != nullptr);
    assert(pAllocHeader != nullptr);

    logging::format(
        "%s(%d)\n"
        "{ heap=\"%s\" address=0x%p size=%zubyte time=%s "
        "backTraceHash=0x%016zX bookmark=%zX }\n"
        "SIGNATURE=[ %08X ]",
        pAllocHeader->getFileName(), pAllocHeader->getLine(), pHeap->getName(),
        pAllocHeader->getBlock(), pAllocHeader->getBytes(),
        /*pAllocation->time*/ "", pAllocHeader->getBackTraceHash(),
        pAllocHeader->getBookmark(), pAllocHeader->getSignature());
}


class TreeStatsReporter : public memory::IHeapTreeStatsReporter
{
public:
    void beginReport() override
    {
        logging::format("MEMORY INFORMATION");
        logging::format(
            "                            Local                 Total");
        logging::format(
            "Name                  Memory  Peak  Inst   Memory  Peak  Inst");
    }

    void report(
        wfl::int32_t depth,
        const memory::Heap* pHeap,
        const HeapTreeStats& local,
        const HeapTreeStats& total) override
    {
        for (int i = 0; i < depth; ++i)
        {
            logging::format("..");
        }

        int spacing = 20 - depth * 2;
        logging::format("%-*s %6zu %6zu %5zu  %6zu %6zu %5zu", spacing,
            pHeap->getName(), local.totalBytes, local.peakBytes,
            local.instanceCount, total.totalBytes, total.peakBytes,
            total.instanceCount);
    }

    void endReport() override
    {}
};


class LeakReporter : public memory::IMemoryLeakReporter
{
public:
    LeakReporter() : m_leakCount(0)
    {}

    void beginReport() override
    {
        m_leakCount = 0;
    }

    void report(
        const memory::Heap* pHeap,
        const memory::AllocHeader* pAllocHeader) override
    {
        printAllocInfo(pHeap, pAllocHeader);
        m_leakCount++;
    }

    void endReport() override
    {
        if (m_leakCount > 0)
        {
            logging::format("%zu memory leaks found!", m_leakCount);
        }
        else
        {
            logging::format("No memory leaks detected.");
        }
    }

public:
    size_t m_leakCount;
};


class AssertionReporter : public memory::IMemoryAssertionReporter
{
public:
    AssertionReporter() : m_assertionCount(0)
    {}

    void beginReport() override
    {
        m_assertionCount = 0;
    }

    void report(
        const memory::Heap* pHeap,
        const memory::AllocHeader* pAllocHeader) override
    {
        printAllocInfo(pHeap, pAllocHeader);
        m_assertionCount++;
    }

    void endReport() override
    {
        if (m_assertionCount > 0)
        {
            logging::format("%zu memory assertions occurred!", m_assertionCount);
        }
        else
        {
            logging::format("No memory assertions detected.");
        }
    }

public:
    size_t m_assertionCount;
};


void RuntimeHeap::printDebug_Report_HeapTreeStats()
{
    TreeStatsReporter statsReporter;
    memory::HeapFactory::get().reportHeapTreeStats(&statsReporter);
}


void RuntimeHeap::printDebug_Report_MemoryLeaks()
{
    LeakReporter leakReporter;
    memory::HeapFactory::get().reportMemoryLeaks(&leakReporter);
}


void RuntimeHeap::printDebug_Report_MemoryAssertions()
{
    AssertionReporter assertionReporter;
    memory::HeapFactory::get().reportMemoryAssertions(&assertionReporter);
}


void RuntimeHeap::printDebug_Report_MemoryAll()
{
    printDebug_Report_HeapTreeStats();
    printDebug_Report_MemoryLeaks();
    printDebug_Report_MemoryAssertions();
}


} // namespace runtime
} // namespace waffle

