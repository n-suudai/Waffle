
#include "system/common/types.h"
#include "system/common/platform/win32.h"
#include "system/memory/global_heap.h"
#include <new>


#define WFL_ENABLE_GLOBAL_NEW_DELETE_LOG 0

#if WFL_ENABLE_GLOBAL_NEW_DELETE_LOG

#define WFL_GLOBAL_NEW_DELETE_LOG(str) OutputDebugStringA(str)

#else

#define WFL_GLOBAL_NEW_DELETE_LOG(str)

#endif


_NODISCARD _Ret_notnull_
_Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR void* __CRTDECL
operator new(size_t _Size)
{
    WFL_GLOBAL_NEW_DELETE_LOG("[operator new(size_t)] is called.\n");

    void* p = WFL_GLOBAL_HEAP_MALLOC(_Size);

    if (p == nullptr)
    {
        throw waffle::wfl::bad_alloc();
    }

    return p;
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL)
_Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR void* __CRTDECL
operator new(size_t _Size, waffle::wfl::nothrow_t const&) noexcept
{
    WFL_GLOBAL_NEW_DELETE_LOG(
        "[operator new(size_t, waffle::wfl::nothrow_t const&) noexcept] is called.\n");

    void* p = WFL_GLOBAL_HEAP_MALLOC(_Size);

    return p;
}

_NODISCARD _Ret_notnull_
_Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR void* __CRTDECL
operator new [](size_t _Size)
{
    WFL_GLOBAL_NEW_DELETE_LOG("[operator new[](size_t)] is called.\n");

    void* p = WFL_GLOBAL_HEAP_MALLOC(_Size);

    if (p == nullptr)
    {
        throw waffle::wfl::bad_alloc();
    }
    return p;
}

_NODISCARD _Ret_maybenull_ _Success_(return != NULL)
_Post_writable_byte_size_(_Size) _VCRT_ALLOCATOR void* __CRTDECL
operator new[](size_t _Size, waffle::wfl::nothrow_t const&) noexcept
{
    WFL_GLOBAL_NEW_DELETE_LOG(
        "[operator new[](size_t, waffle::wfl::nothrow_t const&) noexcept] is called.\n");

    void* p = WFL_GLOBAL_HEAP_MALLOC(_Size);

    return p;
}

void __CRTDECL operator delete(void* _Block) noexcept
{
    WFL_GLOBAL_NEW_DELETE_LOG("[operator delete(void*) noexcept] is called.\n");

    WFL_GLOBAL_HEAP_FREE(_Block);
}

void __CRTDECL operator delete(void* _Block, waffle::wfl::nothrow_t const&) noexcept
{
    WFL_GLOBAL_NEW_DELETE_LOG(
        "[operator delete(void*, waffle::wfl::nothrow_t const&) noexcept] is called.\n");

    WFL_GLOBAL_HEAP_FREE(_Block);
}

void __CRTDECL operator delete[](void* _Block) noexcept
{
    WFL_GLOBAL_NEW_DELETE_LOG("[operator delete[](void*) noexcept] is called.\n");

    WFL_GLOBAL_HEAP_FREE(_Block);
}

void __CRTDECL operator delete[](void* _Block, waffle::wfl::nothrow_t const&) noexcept
{
    WFL_GLOBAL_NEW_DELETE_LOG(
        "[operator delete[](void*, waffle::wfl::nothrow_t const&) noexcept] is "
        "called.\n");

    WFL_GLOBAL_HEAP_FREE(_Block);
}

void __CRTDECL operator delete(void* _Block, size_t) noexcept
{
    WFL_GLOBAL_NEW_DELETE_LOG("[operator delete(void*, size_t) noexcept] is called.\n");

    WFL_GLOBAL_HEAP_FREE(_Block);
}

void __CRTDECL operator delete[](void* _Block, size_t) noexcept
{
    WFL_GLOBAL_NEW_DELETE_LOG("[operator delete[](void*, size_t) noexcept] is called.\n");

    WFL_GLOBAL_HEAP_FREE(_Block);
}


