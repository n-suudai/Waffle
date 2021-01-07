#pragma once


#include "system/memory/memory_manager.h"
#include "system/memory/stl/stl_allocator_with_tracking.h"
#include "system/memory/stl/stl_allocator_without_tracking.h"


namespace waffle {
namespace memory {
namespace detail {


// カスタムデリータ UniquePtr用
template <typename T>
struct CustomDeleter
{
    WFL_NO_NEW_DELETE;

    inline CustomDeleter() = default;

    template <typename T2,
        wfl::enable_if_t<wfl::is_convertible_v<T2*, T*>, int> = 0>
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

    template <typename T2, wfl::enable_if_t<
        wfl::is_convertible_v<T2(*)[], T(*)[]>, int> = 0>
        inline CustomDeleter(const CustomDeleter<T2[]>&)
    { // construct from another default_delete
    }

    template <typename T2, wfl::enable_if_t<
        wfl::is_convertible_v<T2(*)[], T(*)[]>, int> = 0>
        inline void operator()(T2* ptr) const
    { // delete a pointer
        static_assert(0 < sizeof(T2), "can't delete an incomplete type");

        WFL_DELETE[] ptr;
    }
};


} // namespace detail


// UniquePtr
template <typename T, typename Deleter = detail::CustomDeleter<T>>
using UniquePtr = wfl::unique_ptr<T, Deleter>;

// SharedPtr
template <typename T>
using SharedPtr = wfl::shared_ptr<T>;

// WeakPtr
template <typename T>
using WeakPtr = wfl::weak_ptr<T>;


template<typename T, typename... Arguments>
[[nodiscard]] inline UniquePtr<T> makeUnique_WithTracking(
    const char* file,
    wfl::int32_t line,
    const char* function,
    Arguments &&... arguments)
{
    static detail::CustomDeleter<T> deleter = detail::CustomDeleter<T>();

    return UniquePtr<T>(
        new(file, line, function) T(wfl::forward<Arguments>(arguments)...),
        deleter);
}

template<typename T, typename... Arguments>
[[nodiscard]] inline UniquePtr<T> makeUnique_WithoutTracking(Arguments &&... arguments)
{
    static detail::CustomDeleter<T> deleter = detail::CustomDeleter<T>();

    return UniquePtr<T>(
        new T(wfl::forward<Arguments>(arguments)...),
        deleter);
}


template <typename T, typename... Arguments>
[[nodiscard]] inline SharedPtr<T> makeShared_WithTracking(
    Heap* pHeap,
    const char* file,
    wfl::int32_t line,
    const char* function,
    Arguments &&... arguments)
{
    STLAllocator_WithTracking<T> allocator(pHeap, file, line, function);

    return wfl::allocate_shared<T, STLAllocator_WithTracking<T>>(
        allocator, wfl::forward<Arguments>(arguments)...);
}

template <typename T, typename... Arguments>
[[nodiscard]] inline SharedPtr<T> makeShared_WithoutTracking(Arguments &&... arguments)
{
    STLAllocator_WithoutTracking<T> allocator = STLAllocator_WithoutTracking<T>();
    return wfl::allocate_shared<T, STLAllocator_WithoutTracking<T>>(
        allocator, wfl::forward<Arguments>(arguments)...);
}


} // namespace memory


// UniquePtr
template <typename T, typename Deleter = memory::detail::CustomDeleter<T>>
using UniquePtr = memory::UniquePtr<T, Deleter>;

// SharedPtr
template <typename T>
using SharedPtr = memory::SharedPtr<T>;

// WeakPtr
template <typename T>
using WeakPtr = memory::WeakPtr<T>;


} // namespace waffle


