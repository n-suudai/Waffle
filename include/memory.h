#pragma once

#include "memory/global_heap.h"


namespace waffle {
namespace memory {


bool initialize();

void finalize();


} // namespace memory
} // namespace waffle


// メンバ関数を定義

class Hoge
{
private:
    static waffle::memory::Heap* s_pHeap;

protected:
    static void* heapMalloc(waffle::wfl::size_t bytes);
    static void* heapMallocAligned(waffle::wfl::size_t bytes, waffle::wfl::size_t alignment);

    static void heapFree(void* pBlock);
    static void heapFreeAligned(void* pBlock, waffle::wfl::size_t alignment);

public:
    static void* operator new (
        waffle::wfl::size_t bytes,
        const char* filename,
        waffle::wfl::int32_t line,
        const char* function);

    static void* operator new[](
        waffle::wfl::size_t bytes,
        const char* filename,
        waffle::wfl::int32_t line,
        const char* function);

    static void operator delete (
        void* pBlock,
        const char* filename,
        waffle::wfl::int32_t line,
        const char* function);

    static void operator delete[](
        void* pBlock,
        const char* filename,
        waffle::wfl::int32_t line,
        const char* function);
};


