﻿
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
#include "memory/policy/policy_nedmalloc.h"
#include "memory/config.h"
#include <algorithm>


namespace waffle {
namespace memory {


WFL_DECL_MALLOC void* NedAllocatePolicy::allocate(wfl::size_t bytes)
{
    return nedmalloc::StandardAllocator::allocate(bytes);
}

WFL_DECL_MALLOC void* NedAllocatePolicy::allocateAligned(wfl::size_t bytes, wfl::size_t alignment)
{
    return nedmalloc::StandardAllocator::allocateAligned(bytes, alignment);
}

void NedAllocatePolicy::deallocate(void* pBlock)
{
    nedmalloc::StandardAllocator::deallocate(pBlock);
}

void NedAllocatePolicy::deallocateAligned(void* pBlock, wfl::size_t alignment)
{
    nedmalloc::StandardAllocator::deallocateAligned(pBlock, alignment);
}


} // namespace memory
} // namespace waffle

