
/*

This source code was created with reference to Ogre.

(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "third_party/nedmalloc/wfl_nedmalloc.h"

#ifndef _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING

#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING

#endif // _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING

#pragma warning(push)
#pragma warning(disable : 4189)
#pragma warning(disable : 6001)
#pragma warning(disable : 6250)
#pragma warning(disable : 6285)
#pragma warning(disable : 28159)
#pragma warning(disable : 6297)
#pragma warning(disable : 26451)
#pragma warning(disable : 28182)
#pragma warning(disable : 26450)
#pragma warning(disable : 26812)
#pragma warning(disable : 6011)
#pragma warning(disable : 6255)
#pragma warning(disable : 6386)
#pragma warning(disable : 4505)
#pragma warning(disable : 4702)
#pragma warning(disable : 4996)
#pragma warning(disable : 5033)

#include "nedmalloc.c"

#pragma warning(pop)


namespace waffle {
namespace nedmalloc {


const wfl::size_t s_poolCount = 14; // Needs to be greater than 4
void* s_poolFootprint = reinterpret_cast<void*>(0xBB1AA45ABB1AA45A);
nedalloc::nedpool* s_pools[s_poolCount + 1] = { 0 };
nedalloc::nedpool* s_poolsAligned[s_poolCount + 1] = { 0 };


wfl::size_t poolIDFromSize(wfl::size_t a_reqSize)
{
    // Requests size 16 or smaller are allocated at a 4 byte granularity.
    // Requests size 17 or larger are allocated at a 16 byte granularity.
    // With a s_poolCount of 14, requests size 177 or larger go in the default
    // pool.

    // spreadsheet style =IF(B35<=16; FLOOR((B35-1)/4;1); MIN(FLOOR((B35-1)/16;
    // 1) + 3; 14))

    wfl::size_t poolID = 0;

    if (a_reqSize > 0)
    {
        if (a_reqSize <= 16)
        {
            poolID = (a_reqSize - 1) >> 2;
        }
        else
        {
            poolID = wfl::min<wfl::size_t>(((a_reqSize - 1) >> 4) + 3, s_poolCount);
        }
    }

    return poolID;
}


WFL_DECL_MALLOC void* PoolingAllocator::allocate(wfl::size_t bytes)
{
    wfl::size_t poolID = poolIDFromSize(bytes);
    nedalloc::nedpool* pool(0); // A pool pointer of 0 means the default pool.

    if (poolID < s_poolCount)
    {
        if (s_pools[poolID] == 0)
        {
            // Init pool if first use

            s_pools[poolID] = nedalloc::nedcreatepool(0, 8);
            nedalloc::nedpsetvalue(s_pools[poolID],
                s_poolFootprint); // All pools are stamped
                                  // with a footprint
        }

        pool = s_pools[poolID];
    }

    return nedalloc::nedpmalloc(pool, bytes);
}

WFL_DECL_MALLOC void* PoolingAllocator::allocateAligned(wfl::size_t bytes, wfl::size_t alignment)
{
    wfl::size_t poolID = poolIDFromSize(bytes);
    nedalloc::nedpool* pool(0); // A pool pointer of 0 means the default pool.

    if (poolID < s_poolCount)
    {
        if (s_poolsAligned[poolID] == 0)
        {
            // Init pool if first use

            s_poolsAligned[poolID] = nedalloc::nedcreatepool(0, 8);
            nedalloc::nedpsetvalue(s_poolsAligned[poolID],
                s_poolFootprint); // All pools are stamped
                                  // with a footprint
        }

        pool = s_poolsAligned[poolID];
    }

    return nedalloc::nedpmemalign(pool, alignment, bytes);
}

void PoolingAllocator::deallocate(void* pBlock)
{
    if (pBlock)
    {
        nedalloc::nedpool* pool(0);

        // nedalloc lets us get the pool pointer from the memory pointer
        void* footprint = nedalloc::nedgetvalue(&pool, pBlock);

        // Check footprint
        if (footprint == s_poolFootprint)
        {
            // If we allocated the pool, deallocate from this pool...
            nedalloc::nedpfree(pool, pBlock);
        }
        else
        {
            // ...otherwise let nedalloc handle it.
            nedalloc::nedfree(pBlock);
        }
    }
}

void PoolingAllocator::deallocateAligned(void* pBlock, wfl::size_t)
{
    deallocate(pBlock);
}



// standard allocator
WFL_DECL_MALLOC void* StandardAllocator::allocate(wfl::size_t bytes)
{
    return nedalloc::nedmalloc(bytes);
}

WFL_DECL_MALLOC void* StandardAllocator::allocateAligned(wfl::size_t bytes, wfl::size_t alignment)
{
    return nedalloc::nedmemalign(alignment, bytes);
}

void StandardAllocator::deallocate(void* pBlock)
{
    if (pBlock)
    {
        nedalloc::nedfree(pBlock);
    }
}

void StandardAllocator::deallocateAligned(void* pBlock, wfl::size_t)
{
    deallocate(pBlock);
}


} // namespace nedmalloc
} // namespace waffle

