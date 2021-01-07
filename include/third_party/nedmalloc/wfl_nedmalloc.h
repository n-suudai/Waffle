#pragma once


#include "system/common/types.h"
#include "system/common/utility/non_instantiatable.h"
#include "system/common/memory/config.h"


namespace waffle {
namespace nedmalloc {


class PoolingAllocator final
{
    WFL_NON_INSTANTIATABLE(PoolingAllocator);
public:
    [[nodiscard]] static WFL_DECL_MALLOC void* allocate(wfl::size_t bytes);
    [[nodiscard]] static WFL_DECL_MALLOC void* allocateAligned(wfl::size_t bytes, wfl::size_t alignment);

    static void deallocate(void* pBlock);
    static void deallocateAligned(void* pBlock, wfl::size_t alignment);
};


class StandardAllocator final
{
    WFL_NON_INSTANTIATABLE(StandardAllocator);
public:
    [[nodiscard]] static WFL_DECL_MALLOC void* allocate(wfl::size_t bytes);
    [[nodiscard]] static WFL_DECL_MALLOC void* allocateAligned(wfl::size_t bytes, wfl::size_t alignment);

    static void deallocate(void* pBlock);
    static void deallocateAligned(void* pBlock, wfl::size_t alignment);
};


} // namespace nedmalloc
} // namespace waffle

