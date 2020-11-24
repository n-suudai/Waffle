#pragma once


#define WFL_ALLOC_POLICY_NED_POOLING 1
#define WFL_ALLOC_POLICY_NED 2
#define WFL_ALLOC_POLICY_STANDARD 3

#define WFL_ALLOC_POLICY WFL_ALLOC_POLICY_NED_POOLING
//#define WFL_ALLOC_POLICY WFL_ALLOC_POLICY_NED
//#define WFL_ALLOC_POLICY WFL_ALLOC_POLICY_STANDARD

#define WFL_SIMD_ALIGNMENT 128
#define WFL_USE_HEAP_TRACKING 1 // Heapトラッキング機能を使用

#ifndef WFL_DECL_MALLOC

#if defined(_MSC_VER)

#define WFL_DECL_MALLOC __declspec(restrict) __declspec(noalias)

#else // !defined(_MSC_VER)

#define WFL_DECL_MALLOC __attribute__((malloc))

#endif

#endif // WFL_DECL_MALLOC

