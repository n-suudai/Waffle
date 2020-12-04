#pragma once


#include "common/utility/no_new_delete.h"
#include "memory/heap.h"
#include <memory>


namespace waffle {
namespace memory {


// カスタムデリータ UniquePtr用
template <typename T>
struct CustomDeleter
{
    WFL_NO_NEW_DELETE;

    inline CustomDeleter() = default;

    template <typename T2,
        std::enable_if_t<std::is_convertible_v<T2*, T*>, int> = 0>
        inline CustomDeleter(const CustomDeleter<T2>&)
    { // construct from another default_delete
    }

    inline void operator()(T* ptr) const
    {
        static_assert(0 < sizeof(T), "can't delete an incomplete type");

        WFL_DELETE ptr;
    }
};

// カスタムデリータ 配列型のUniquePtr用
template <typename T>
struct CustomDeleter<T[]>
{
    WFL_NO_NEW_DELETE;

    inline CustomDeleter() = default;

    template <typename T2, std::enable_if_t<
        std::is_convertible_v<T2(*)[], T(*)[]>, int> = 0>
        inline CustomDeleter(const CustomDeleter<T2[]>&)
    { // construct from another default_delete
    }

    template <typename T2, std::enable_if_t<
        std::is_convertible_v<T2(*)[], T(*)[]>, int> = 0>
        inline void operator()(T2* ptr) const
    { // delete a pointer
        static_assert(0 < sizeof(T2), "can't delete an incomplete type");

        WFL_DELETE[] ptr;
    }
};


// カスタムデリータ UniqueBlob用
template <typename T>
struct CustomDeleter_Blob
{
    inline CustomDeleter_Blob(Heap* pHeap) : m_pHeap(pHeap)
    {}

    inline CustomDeleter_Blob() = default;

    inline Heap* getHeap() const
    {
        return m_pHeap;
    }

    template <typename T2,
        std::enable_if_t<std::is_convertible_v<T2*, T*>, int> = 0>
        inline CustomDeleter_Blob(const CustomDeleter_Blob<T2>&)
    { // construct from another default_delete
    }

    inline void operator()(T* ptr) const
    {
        static_assert(0 < sizeof(T), "can't delete an incomplete type");

        NE_MEM_FREE_HEAP(ptr);
    }

    Heap* m_pHeap;
};


} // namespace memory
} // namespace waffle

