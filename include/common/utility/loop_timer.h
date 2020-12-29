#pragma once

#include "common/types.h"
#include "common/utility/no_new_delete.h"
#include <chrono>


namespace waffle {


class LoopTimer final
{
    WFL_NO_NEW_DELETE;
public:
    typedef wfl::chrono::high_resolution_clock Clock;
    typedef Clock::time_point TimePoint;

public:
    inline LoopTimer()
        : m_lastLoopStarted(Clock::now())
    {}

    inline ~LoopTimer() = default;

    template<typename Duration>
    inline Duration update()
    {
        using namespace wfl::chrono;

        TimePoint now = Clock::now();
        
        Duration elapsedDuration = duration_cast<Duration>(m_lastLoopStarted - now);

        m_lastLoopStarted = now;
        
        return elapsedDuration;
    }

private:
    TimePoint m_lastLoopStarted;
};


} // namespace waffle

