#pragma once


#include "common/utility/no_new_delete.h"
#include "memory/policy.h"
#include "memory/stl/stl_heap_factory.h"


namespace waffle {
namespace memory {


// 追跡あり
template<typename T>
struct STLAllocator_WithTracking final
{
    WFL_NO_NEW_DELETE;

    static Heap* s_pHeapSTL;
    Heap* m_pHeap;

    [[nodiscard]] inline Heap* getHeap()
    {
        if (m_pHeap) { return m_pHeap; }

        if (!s_pHeapSTL)
        {
            s_pHeapSTL = getHeapSTL();
        }
        return s_pHeapSTL;
    }

    using value_type = T;
    static const wfl::size_t ALIGNMENT;

    template <typename U>
    struct rebind
    {
        typedef STLAllocator_WithTracking<U> other;
    };

    inline STLAllocator_WithTracking(
        Heap* pHeap,
        const char* file,
        wfl::int32_t line,
        const char* function)
        : m_pHeap(pHeap)
        , m_file(file)
        , m_line(line)
        , m_function(function)
    {/* DO_NOTHING */
    }

    inline STLAllocator_WithTracking()
        : m_pHeap(nullptr)
        , m_file(nullptr)
        , m_line(0)
        , m_function(nullptr)
    {/* DO_NOTHING */
    }

    template <typename U>
    inline STLAllocator_WithTracking(const STLAllocator_WithTracking<U>& other)
    {
        // コピー
        this->m_pHeap = other.m_pHeap;
        this->m_file = other.m_file;
        this->m_line = other.m_line;
        this->m_function = other.m_function;
    }

    [[nodiscard]] inline T* allocate(wfl::size_t count)
    {
        if (m_pHeap)
        {
            return reinterpret_cast<T*>(
                m_pHeap->allocateAligned<AllocatePolicy>(
                    sizeof(T) * count,
                    ALIGNMENT,
                    m_file,
                    m_line,
                    m_function));
        }

        return reinterpret_cast<T*>(
            getHeap()->allocateAligned<AllocatePolicy>(
                sizeof(T) * count,
                ALIGNMENT,
                __FILE__,
                __LINE__,
                __FUNCTION__));
    }

    inline void deallocate(T* ptr, wfl::size_t)
    {
        getHeap()->deallocateAligned<AllocatePolicy>(ptr, ALIGNMENT);
    }

    const char* m_file;
    wfl::int32_t m_line;
    const char* m_function;
};

template<typename T>
Heap* STLAllocator_WithTracking<T>::s_pHeapSTL = nullptr;

template<typename T>
const wfl::size_t STLAllocator_WithTracking<T>::ALIGNMENT = alignof(T);

template <typename T, typename U>
[[nodiscard]] inline bool operator==(const STLAllocator_WithTracking<T>&, const STLAllocator_WithTracking<U>&)
{
    return true;
}

template <typename T, typename U>
[[nodiscard]] inline bool operator!=(const STLAllocator_WithTracking<T>&, const STLAllocator_WithTracking<U>&)
{
    return false;
}


} // namespace memory
} // namespace waffle