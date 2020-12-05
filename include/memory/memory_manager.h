#pragma once


#include "memory/stl/stl_blob.h"
#include "memory/heap_factory.h"
#include "memory/policy.h"


#if WFL_USE_HEAP_TRACKING

#define WFL_DECLARE_HEAP \
private: \
static waffle::memory::Heap* s_pHeap; \
static waffle::memory::Heap* getHeap(); \
 \
protected: \
    static waffle::memory::UniqueBlob makeUniqueBlob( \
        waffle::wfl::size_t bytes, \
        const char* filename, \
        waffle::wfl::int32_t line, \
        const char* function); \
 \
    static waffle::memory::UniqueBlob makeUniqueBlobAligned( \
        waffle::wfl::size_t bytes, \
        waffle::wfl::size_t alignment, \
        const char* filename, \
        waffle::wfl::int32_t line, \
        const char* function); \
 \
template<typename T, typename... Arguments> \
static inline waffle::memory::SharedPtr<T> makeShared( \
    const char* file, \
    waffle::wfl::int32_t line, \
    const char* function, \
    Arguments &&... arguments) \
{ \
    return waffle::memory::makeShared_WithTracking<T>( \
        getHeap(), \
        file, \
        line, \
        function, \
        std::forward<Arguments>(arguments)...); \
} \
 \
template<typename T, typename... Arguments> \
static inline waffle::memory::UniquePtr<T> makeUnique( \
    const char* file, \
    waffle::wfl::int32_t line, \
    const char* function, \
    Arguments &&... arguments) \
{ \
    return waffle::memory::makeUnique_WithTracking( \
        file, \
        line, \
        function, \
        std::forward<Arguments>(arguments)...); \
} \
 \
public: \
    static void* operator new ( \
        waffle::wfl::size_t bytes, \
        const char* filename, \
        waffle::wfl::int32_t line, \
        const char* function); \
 \
    static void* operator new[]( \
        waffle::wfl::size_t bytes, \
        const char* filename, \
        waffle::wfl::int32_t line, \
        const char* function); \
 \
    static void operator delete ( \
        void* pBlock, \
        const char* filename, \
        waffle::wfl::int32_t line, \
        const char* function); \
 \
    static void operator delete[]( \
        void* pBlock, \
        const char* filename, \
        waffle::wfl::int32_t line, \
        const char* function); \


#define WFL_DEFINE_HEAP_BASE(class_name) \
waffle::memory::Heap* class_name::s_pHeap = nullptr; \
 \
waffle::memory::UniqueBlob class_name::makeUniqueBlob( \
    waffle::wfl::size_t bytes, \
    const char* filename, \
    waffle::wfl::int32_t line, \
    const char* function) \
{ \
    return waffle::memory::makeUniqueBlob_WithTracking( \
        bytes, \
        getHeap(), \
        filename, \
        line, \
        function); \
} \
 \
waffle::memory::UniqueBlob class_name::makeUniqueBlobAligned( \
    waffle::wfl::size_t bytes, \
    waffle::wfl::size_t alignment, \
    const char* filename, \
    waffle::wfl::int32_t line, \
    const char* function) \
{ \
    return waffle::memory::makeUniqueBlobAligned_WithTracking( \
        bytes, \
        alignment, \
        getHeap(), \
        filename, \
        line, \
        function); \
} \
 \
void* class_name::operator new ( \
    waffle::wfl::size_t bytes, \
    const char* filename, \
    waffle::wfl::int32_t line, \
    const char* function) \
{ \
    return getHeap()->allocate<waffle::memory::AllocatePolicy>( \
        bytes, \
        filename, \
        line, \
        function); \
} \
 \
void* class_name::operator new[]( \
    waffle::wfl::size_t bytes, \
    const char* filename, \
    waffle::wfl::int32_t line, \
    const char* function) \
{ \
    return getHeap()->allocate<waffle::memory::AllocatePolicy>( \
        bytes, \
        filename, \
        line, \
        function); \
} \
 \
void class_name::operator delete ( \
    void* pBlock, \
    const char*, \
    waffle::wfl::int32_t, \
    const char*) \
{ \
    getHeap()->deallocate<waffle::memory::AllocatePolicy>(pBlock); \
} \
 \
void class_name::operator delete[]( \
    void* pBlock, \
    const char*, \
    waffle::wfl::int32_t, \
    const char*) \
{ \
    getHeap()->deallocate<waffle::memory::AllocatePolicy>(pBlock); \
} \


#define WFL_DEFINE_HEAP(class_name, heap_name) \
WFL_DEFINE_HEAP_BASE(class_name) \
waffle::memory::Heap* class_name::getHeap() \
{ \
    if (!s_pHeap) \
    { \
        s_pHeap = waffle::memory::HeapFactory::get().createHeap(heap_name); \
    } \
    return s_pHeap; \
} \

#define WFL_DEFINE_HIERARCHAL_HEAP(class_name, heap_name, parent_heap_name) \
WFL_DEFINE_HEAP_BASE(class_name) \
waffle::memory::Heap* class_name::getHeap() \
{ \
    if (!s_pHeap) \
    { \
        s_pHeap = waffle::memory::HeapFactory::get().createHeap(heap_name, parent_heap_name); \
    } \
    return s_pHeap; \
} \

#define WFL_NEW new(__FILE__, __LINE__, __FUNCTION__)
#define WFL_DELETE delete

#define WFL_MAKE_UNIQUE_BLOB(bytes) \
    makeUniqueBlob((bytes), __FILE__, __LINE__, __FUNCTION__)

#define WFL_MAKE_UNIQUE_BLOB_ALIGNED(bytes, alignment) \
    makeUniqueBlob((bytes), (alignment), __FILE__, __LINE__, __FUNCTION__)


#else

#define WFL_DECLARE_HEAP \
protected: \
static waffle::memory::UniqueBlob makeUniqueBlob( \
    waffle::wfl::size_t bytes); \
 \
static waffle::memory::UniqueBlob makeUniqueBlobAligned( \
    waffle::wfl::size_t bytes, \
    waffle::wfl::size_t alignment); \
 \
template<typename T, typename... Arguments> \
static inline waffle::memory::SharedPtr<T> makeShared(Arguments &&... arguments) \
{ \
    return waffle::memory::makeShared_WithoutTracking<T>( \
        std::forward<Arguments>(arguments)...); \
} \
 \
template<typename T, typename... Arguments> \
static inline waffle::memory::UniquePtr<T> makeUnique(Arguments &&... arguments) \
{ \
    return waffle::memory::makeUnique_WithoutTracking( \
        std::forward<Arguments>(arguments)...); \
} \
 \
public: \
    static void* operator new(waffle::wfl::size_t bytes); \
 \
    static void* operator new[](waffle::wfl::size_t bytes); \
 \
    static void operator delete(void* pBlock); \
 \
    static void operator delete[](void* pBlock); \


#define WFL_DEFINE_HEAP_BASE(class_name) \
waffle::memory::UniqueBlob class_name::makeUniqueBlob( \
    waffle::wfl::size_t bytes) \
{ \
    return waffle::memory::makeUniqueBlob_WithoutTracking(bytes); \
} \
 \
waffle::memory::UniqueBlob class_name::makeUniqueBlobAligned( \
    waffle::wfl::size_t bytes, \
    waffle::wfl::size_t alignment) \
{ \
    return waffle::memory::makeUniqueBlobAligned_WithoutTracking( \
        bytes, \
        alignment); \
} \
 \
void* class_name::operator new(waffle::wfl::size_t bytes) \
{ \
    return waffle::memory::AllocatePolicy::allocate(bytes); \
} \
 \
void* class_name::operator new[](waffle::wfl::size_t bytes) \
{ \
    return waffle::memory::AllocatePolicy::allocate(bytes); \
} \
 \
void class_name::operator delete(void* pBlock) \
{ \
    waffle::memory::AllocatePolicy::deallocate(pBlock); \
} \
 \
void class_name::operator delete[](void* pBlock) \
{ \
    waffle::memory::AllocatePolicy::deallocate(pBlock); \
} \


#define WFL_DEFINE_HEAP(class_name, heap_name) \
WFL_DEFINE_HEAP_BASE(class_name) \


#define WFL_DEFINE_HIERARCHAL_HEAP(class_name, heap_name, parent_heap_name) \
WFL_DEFINE_HEAP_BASE(class_name) \


#define WFL_NEW new
#define WFL_DELETE delete

#define WFL_MAKE_UNIQUE_BLOB(bytes) \
    makeUniqueBlob((bytes))

#define WFL_MAKE_UNIQUE_BLOB_ALIGNED(bytes, alignment) \
    makeUniqueBlob((bytes), (alignment))


#endif

