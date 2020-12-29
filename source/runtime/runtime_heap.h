#pragma once

#include "common/utility/non_instantiatable.h"


namespace waffle {
namespace runtime {


class RuntimeHeap
{
    WFL_NON_INSTANTIATABLE(RuntimeHeap);
public:
    static void printDebug_Report_MemoryAll();
    static void printDebug_Report_HeapTreeStats();
    static void printDebug_Report_MemoryLeaks();
    static void printDebug_Report_MemoryAssertions();

};


} // namespace runtime
} // namespace waffle

