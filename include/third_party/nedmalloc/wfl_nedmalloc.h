#pragma once


#include "common/types.h"
#include "common/utility/non_instantiatable.h"
#include "common/memory/config.h"


namespace waffle {
namespace nedmalloc {


class PoolingAllocator final
{
    WFL_NON_INSTANTIATABLE(PoolingAllocator);
public:
    static WFL_DECL_MALLOC void* allocate(wfl::size_t bytes);
    static WFL_DECL_MALLOC void* allocateAligned(wfl::size_t bytes, wfl::size_t alignment);

    static void deallocate(void* pBlock);
    static void deallocateAligned(void* pBlock, wfl::size_t alignment);
};


class StandardAllocator final
{
    WFL_NON_INSTANTIATABLE(StandardAllocator);
public:
    static WFL_DECL_MALLOC void* allocate(wfl::size_t bytes);
    static WFL_DECL_MALLOC void* allocateAligned(wfl::size_t bytes, wfl::size_t alignment);

    static void deallocate(void* pBlock);
    static void deallocateAligned(void* pBlock, wfl::size_t alignment);
};


} // namespace nedmalloc
} // namespace waffle

