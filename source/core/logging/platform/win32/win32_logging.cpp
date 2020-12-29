
#include "core/logging.h"
#include "common/platform.h"
#include <cstdio>
#include <cstdarg>


namespace waffle {
namespace logging {


void put(const char* text, bool newLine)
{
    ::OutputDebugStringA(text);

    if (newLine)
    {
        ::OutputDebugStringA("\n");
    }
}

void format(const char* formatString, ...)
{
    if (strlen(formatString) < 1) { return; }

    char buffer[1024];
    {
        wfl::va_list vlist;
        va_start(vlist, formatString);
        vsnprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, formatString, vlist);
        va_end(vlist);
    }

    waffle::logging::put(buffer);
}

void formatNoNewLine(const char* formatString, ...)
{
    if (strlen(formatString) < 1) { return; }

    char buffer[1024];
    {
        wfl::va_list vlist;
        va_start(vlist, formatString);
        vsnprintf_s(buffer, ARRAYSIZE(buffer), _TRUNCATE, formatString, vlist);
        va_end(vlist);
    }

    waffle::logging::put(buffer, false);
}


} // namespace logging
} // namespace waffle

