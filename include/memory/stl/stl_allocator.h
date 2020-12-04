#pragma once


#include "common/utility/no_new_delete.h"
#include "memory/heap_factory.h"
#include "memory/policy.h"


namespace waffle {
namespace memory {


// 追跡あり
template<typename T>
struct STLAllocator_WithTracking final
{
    WFL_NO_NEW_DELETE;

    static Heap* s_pHeapSTL;
    Heap* m_pHeap;

    Heap* getHeap()
    {
        if (m_pHeap) { return m_pHeap; }

        if (!s_pHeapSTL)
        {
            s_pHeapSTL = HeapFactory::get().createHeap("STL");
        }
        return s_pHeap;
    }

    using value_type = T;
    constexpr wfl::size_t ALIGNMENT = alignof(T);

    template <typename U>
    struct rebind
    {
        typedef STLAllocator_WithTracking<U> other;
    };

    inline STLAllocator_WithTracking(Heap* pHeap) : m_pHeap(pHeap)
    {/* DO_NOTHING */
    }

    inline STLAllocator_WithTracking() : m_pHeap(nullptr)
    {/* DO_NOTHING */
    }

    template <typename U>
    inline STLAllocator_WithTracking(const STLAllocator_WithTracking<U>&)
    {/* DO_NOTHING */
    }

    inline T* allocate(wfl::size_t count)
    {
        return reinterpret_cast<T*>(
            getHeap()->allocateAligned(
                sizeof(T) * count,
                ALIGNMENT,
                __FILE__,
                __LINE__,
                __FUNCTION__));
    }

    inline void deallocate(T* ptr, wfl::size_t)
    {
        getHeap()->deallocate(ptr);
    }
};


// 追跡なし
template<typename T>
struct STLAllocator_WithoutTracking final
{
    WFL_NO_NEW_DELETE;

    using value_type = T;
    constexpr wfl::size_t ALIGNMENT = alignof(T);

    template <typename U>
    struct rebind
    {
        typedef STLAllocator_WithoutTracking<U> other;
    };

    inline STLAllocator_WithoutTracking()
    {/* DO_NOTHING */
    }

    template <typename U>
    inline STLAllocator_WithoutTracking(const STLAllocator_WithoutTracking<U>&)
    {/* DO_NOTHING */
    }

    inline T* allocate(wfl::size_t count)
    {
        return reinterpret_cast<T*>(
            AllocatePolicy::allocateAligned(sizeof(T) * count, ALIGNMENT));
    }

    inline void deallocate(T* ptr, wfl::size_t)
    {
        AllocatePolicy::deallocateAligned(ptr, ALIGNMENT);
    }
};

template <typename T, typename U>
inline bool operator==(const STLAllocator_WithoutTracking<T>&, const STLAllocator_WithoutTracking<U>&)
{
    return true;
}

template <typename T, typename U>
inline bool operator!=(const STLAllocator_WithoutTracking<T>&, const STLAllocator_WithoutTracking<U>&)
{
    return false;
}


#if WFL_USE_HEAP_TRACKING

template<typename T>
using STLAllocator = STLAllocator_WithTracking<T>;

#else

template<typename T>
using STLAllocator = STLAllocator_WithoutTracking<T>;

#endif


} // namespace memory
} // namespace waffle





