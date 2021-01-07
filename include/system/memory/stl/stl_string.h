#pragma once


#include "system/memory/stl/stl_allocator.h"
#include <string>
#include <sstream>


namespace waffle {
namespace memory {


// BasicString
template <typename Elem, typename Traits = wfl::char_traits<Elem>,
    typename Alloc = STLAllocator<Elem>>
    using BasicString = wfl::basic_string<Elem, Traits, Alloc>;

// String
typedef BasicString<char> String;

// BasicStringStream
template <typename Elem, typename Traits = wfl::char_traits<Elem>,
    typename Alloc = STLAllocator<Elem>>
    using BasicStringStream = wfl::basic_stringstream<Elem, Traits, Alloc>;

// StringStream
typedef BasicStringStream<char> StringStream;

// BasicIStringStream
template <typename Elem, typename Traits = wfl::char_traits<Elem>,
    typename Alloc = STLAllocator<Elem>>
    using BasicIStringStream = wfl::basic_istringstream<Elem, Traits, Alloc>;

// IStringStream
typedef BasicIStringStream<char> IStringStream;

// BasicOStringStream
template <typename Elem, typename Traits = wfl::char_traits<Elem>,
    typename Alloc = STLAllocator<Elem>>
    using BasicOStringStream = wfl::basic_ostringstream<Elem, Traits, Alloc>;

// OStringStream
typedef BasicOStringStream<char> OStringStream;


// BasicStringView
template<typename Elem, typename Traits = wfl::char_traits<Elem>>
using BasicStringView = wfl::basic_string_view<Elem, Traits>;

// StringView
typedef BasicStringView<char> StringView;


} // namespace memory


using memory::String;
using memory::StringStream;
using memory::IStringStream;
using memory::OStringStream;
using memory::StringView;


} // namespace waffle

