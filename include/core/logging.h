#pragma once


namespace waffle {
namespace logging {


void put(const char* text, bool newLine = true);
void format(const char* str, ...);
void formatNoNewLine(const char* str, ...);


} // namespace logging
} // namespace waffle

