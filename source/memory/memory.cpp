#include <cassert>
#include "memory.h"
#include "memory/alloc_header.h"
#include "memory/heap_walk.h"
#include "memory/heap_factory.h"


namespace waffle {
namespace memory {


bool initialize()
{
    HeaderInfoFlags flags = HeaderInfoFlagBits::All;
    bool result =  AllocHeader::initialize(flags);

    return result;
}

void finalize()
{
    AllocHeader::finalize();
}


} // namespace memory
} // namespace waffle

