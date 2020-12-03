#pragma once

#define WFL_SIMD_ALIGNMENT 128

#ifndef WFL_DECL_MALLOC

#if defined(_MSC_VER)

#define WFL_DECL_MALLOC __declspec(restrict) __declspec(noalias)

#else // !defined(_MSC_VER)

#define WFL_DECL_MALLOC __attribute__((malloc))

#endif

#endif // WFL_DECL_MALLOC

