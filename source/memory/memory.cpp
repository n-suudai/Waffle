#include "memory.h"
#include "memory/alloc_header.h"


namespace waffle {
namespace memory {


bool initialize()
{
    bool result =  AllocHeader::initialize(HeaderInfoFlagBits::Required);
    return result;
}

void finalize()
{

}


} // namespace memory
} // namespace waffle

