#pragma once


#include "memory/stl/stl_allocator.h"
#include <string>
#include <sstream>


namespace waffle {
namespace memory {


// BasicString
template <typename Elem, typename Traits = std::char_traits<Elem>,
    typename Alloc = STLAllocator<Elem>>
    using BasicString = std::basic_string<Elem, Traits, Alloc>;

// String
typedef BasicString<char> String;

// BasicStringStream
template <typename Elem, typename Traits = std::char_traits<Elem>,
    typename Alloc = STLAllocator<Elem>>
    using BasicStringStream = std::basic_stringstream<Elem, Traits, Alloc>;

// StringStream
typedef BasicStringStream<char> StringStream;

// BasicIStringStream
template <typename Elem, typename Traits = std::char_traits<Elem>,
    typename Alloc = STLAllocator<Elem>>
    using BasicIStringStream = std::basic_istringstream<Elem, Traits, Alloc>;

// IStringStream
typedef BasicIStringStream<char> IStringStream;

// BasicOStringStream
template <typename Elem, typename Traits = std::char_traits<Elem>,
    typename Alloc = STLAllocator<Elem>>
    using BasicOStringStream = std::basic_ostringstream<Elem, Traits, Alloc>;

// OStringStream
typedef BasicOStringStream<char> OStringStream;


} // namespace memory


using memory::String;
using memory::StringStream;
using memory::IStringStream;
using memory::OStringStream;


} // namespace waffle

