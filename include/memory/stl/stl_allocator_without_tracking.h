#pragma once


#include "common/utility/no_new_delete.h"
#include "memory/policy.h"


namespace waffle {
namespace memory {


// 追跡なし
template<typename T>
struct STLAllocator_WithoutTracking final
{
    WFL_NO_NEW_DELETE;

    using value_type = T;
    static const wfl::size_t ALIGNMENT;

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

template<typename T>
const wfl::size_t STLAllocator_WithoutTracking<T>::ALIGNMENT = alignof(T);

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


}
}