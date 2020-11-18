#include "core/memory/memory.h"
#include "core/memory/alloc_header.h"


namespace Engine {
namespace Memory {


EBool Initialize()
{
    EBool result =  AllocHeader::Initialize(HeaderInfoFlagBits::Required);
    return result;
}

EVoid Finalize()
{

}


} // namespace Memory
} // namespace Engine

