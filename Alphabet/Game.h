#pragma once

#include <chrono>

#include <tchar.h>

#include "Application.h"

class Game
{
public:
    ~Game() = default;

    const ::_TCHAR& CurrentTarget() const;
    bool Started() const;
    bool SequenceDone() const;
    ::std::chrono::duration<double> Elapsed() const;

    void Update();
    void Input(::_TCHAR ch, ::TShiftState shift);
private:
    ::_TCHAR m_currentTarget = 'a';
    bool m_started = false;
    bool m_sequenceDone = false;
    ::std::chrono::steady_clock::time_point m_startTime;
    ::std::chrono::duration<double> m_elapsed{ 0.0 };
};
