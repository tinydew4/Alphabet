#include "Game.h"

const ::_TCHAR& Game::CurrentTarget() const
{
    return m_currentTarget;
}

bool Game::Started() const
{
    return m_started;
}

bool Game::SequenceDone() const
{
    return m_sequenceDone;
}

::std::chrono::duration<double> Game::Elapsed() const
{
    return m_elapsed;
}

void Game::Update()
{
    if (m_started && !m_sequenceDone)
    {
        m_elapsed = ::std::chrono::steady_clock::now() - m_startTime;
    }
}

void Game::Input(::_TCHAR ch, ::TShiftState shift)
{
    if (m_sequenceDone)
    {
        m_currentTarget = 'a';
        m_started = false;
        m_sequenceDone = false;
        m_elapsed = std::chrono::duration<double>(0.0);
        return;
    }

    if ((::std::isupper(ch)) ? static_cast<_TCHAR>(::std::tolower(ch)) : ch == m_currentTarget)
    {
        if (!m_started && m_currentTarget == 'a')
        {
            m_started = true;
            m_startTime = ::std::chrono::steady_clock::now();
            m_elapsed = std::chrono::duration<double>(0.0);
        }

        if (m_currentTarget == 'z')
        {
            m_sequenceDone = true;
        }
        else
        {
            m_currentTarget++;
        }
    }
}
