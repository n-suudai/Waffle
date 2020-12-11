#pragma once


#include "common/utility/no_new_delete.h"
#include "memory/heap.h"
#include <memory>
#include <cassert>


namespace waffle {
namespace memory {
namespace detail {


// カスタムデリータ UniqueBlob用
template <typename T>
struct CustomDeleter_Blob_WithTracking
{
    WFL_NO_NEW_DELETE;

    inline CustomDeleter_Blob_WithTracking(Heap* pHeap, wfl::size_t alignment = 0)
        : m_pHeap(pHeap), m_alignment(alignment)
    {}

    inline CustomDeleter_Blob_WithTracking() = default;

    inline Heap* getHeap() const
    {
        return m_pHeap;
    }

    template <typename T2,
        std::enable_if_t<std::is_convertible_v<T2*, T*>, int> = 0>
        inline CustomDeleter_Blob_WithTracking(const CustomDeleter_Blob_WithTracking<T2>&)
    { // construct from another default_delete
    }

    inline void operator()(T* ptr) const
    {
        static_assert(0 < sizeof(T), "can't delete an incomplete type");

        if (m_alignment)
        {
            m_pHeap->deallocateAligned<AllocatePolicy>(ptr, m_alignment);
        }
        else
        {
            m_pHeap->deallocate<AllocatePolicy>(ptr);
        }

    }

    Heap* m_pHeap;
    wfl::size_t m_alignment;
};


template <typename T>
struct CustomDeleter_Blob_WithoutTracking
{
    WFL_NO_NEW_DELETE;

    inline CustomDeleter_Blob_WithoutTracking(wfl::size_t alignment = 0)
        : m_alignment(alignment)
    {}

    template <typename T2,
        std::enable_if_t<std::is_convertible_v<T2*, T*>, int> = 0>
        inline CustomDeleter_Blob_WithoutTracking(const CustomDeleter_Blob_WithoutTracking<T2>&)
    { // construct from another default_delete
    }

    inline void operator()(T* ptr) const
    {
        static_assert(0 < sizeof(T), "can't delete an incomplete type");

        if (m_alignment)
        {
            AllocatePolicy::deallocateAligned(ptr, m_alignment);
        }
        else
        {
            AllocatePolicy::deallocate(ptr);
        }
    }

    wfl::size_t m_alignment;
};


} // naemspace detail


// UniqueBlob
using UniqueBlob_WithTracking =
std::unique_ptr<wfl::uint8_t, detail::CustomDeleter_Blob_WithTracking<wfl::uint8_t>>;

using UniqueBlob_WithoutTracking =
std::unique_ptr<wfl::uint8_t, detail::CustomDeleter_Blob_WithoutTracking<wfl::uint8_t>>;


inline UniqueBlob_WithTracking makeUniqueBlob_WithTracking(
    wfl::size_t bytes,
    Heap* pHeap,
    const char* file,
    wfl::int32_t line,
    const char* function)
{
    detail::CustomDeleter_Blob_WithTracking<wfl::uint8_t> deleter(pHeap);

    void* pBlock = pHeap->allocate<AllocatePolicy>(
        bytes,
        file,
        line,
        function);

    return UniqueBlob_WithTracking(
        reinterpret_cast<wfl::uint8_t*>(pBlock),
        deleter);
}

inline UniqueBlob_WithTracking makeUniqueBlobAligned_WithTracking(
    wfl::size_t bytes,
    wfl::size_t alignment,
    Heap* pHeap,
    const char* file,
    wfl::int32_t line,
    const char* function)
{
    assert(alignment);

    detail::CustomDeleter_Blob_WithTracking<wfl::uint8_t> deleter(pHeap, alignment);

    void* pBlock = pHeap->allocateAligned<AllocatePolicy>(
        bytes,
        alignment,
        file,
        line,
        function);

    return UniqueBlob_WithTracking(
        reinterpret_cast<wfl::uint8_t*>(pBlock),
        deleter);
}

inline UniqueBlob_WithoutTracking makeUniqueBlob_WithoutTracking(wfl::size_t bytes)
{
    detail::CustomDeleter_Blob_WithoutTracking<wfl::uint8_t> deleter;

    return UniqueBlob_WithoutTracking(
        reinterpret_cast<wfl::uint8_t*>(AllocatePolicy::allocate(bytes)),
        deleter);
}

inline UniqueBlob_WithoutTracking makeUniqueBlobAligned_WithoutTracking(
    wfl::size_t bytes,
    wfl::size_t alignment)
{
    assert(alignment);

    detail::CustomDeleter_Blob_WithoutTracking<wfl::uint8_t> deleter(alignment);

    return UniqueBlob_WithoutTracking(
        reinterpret_cast<wfl::uint8_t*>(AllocatePolicy::allocateAligned(bytes, alignment)),
        deleter);
}


#if WFL_USE_HEAP_TRACKING

typedef UniqueBlob_WithTracking UniqueBlob;

#else

typedef UniqueBlob_WithoutTracking UniqueBlob;

#endif


} // namespace memory


using memory::UniqueBlob;


} // namespace waffle

