#include "Application.h"

::TShiftState GetShiftState(::DWORD ctrlState)
{
    ::TShiftState shift = { 0 };
    if (ctrlState & SHIFT_PRESSED)
    {
        shift |= ssShift;
    }
    if (ctrlState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
    {
        shift |= ssCtrl;
    }
    if (ctrlState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
    {
        shift |= ssAlt;
    }
    return shift;
}

const ::std::unique_ptr<::Console>& Application::Console()
{
    return m_console;
}

const ::SMALL_RECT& Application::ClientRect() const
{
    return m_clientRect;
}

void Application::Terminate()
{
    m_terminated = true;
}

void Application::ProcessMessages()
{
    ::DWORD numEvents = { 0 };
    if (!::GetNumberOfConsoleInputEvents(Console()->InputHandle(), &numEvents))
    {
        Terminate();
        return;
    }
    for (auto a = numEvents; a-- > 0; ProcessMessage());
}

void Application::ProcessMessage()
{
    ::INPUT_RECORD rec;
    ::DWORD read = 0;
    if (!::ReadConsoleInput(Console()->InputHandle(), &rec, 1, &read) || read == 0)
    {
        return;
    }

    switch (rec.EventType)
    {
    case KEY_EVENT: KeyEvent(rec.Event.KeyEvent); break;
    case WINDOW_BUFFER_SIZE_EVENT: WindowBufferSizeEvent(rec.Event.WindowBufferSizeEvent); break;
    case FOCUS_EVENT: FocusEvent(rec.Event.FocusEvent); break;
    }
}

void Application::KeyEvent(const ::KEY_EVENT_RECORD& event)
{
    const auto shift = GetShiftState(event.dwControlKeyState);
    if (event.bKeyDown != FALSE)
    {
        const auto repeat = (event.wRepeatCount == 0) ? 1 : event.wRepeatCount;
#ifdef UNICODE
        const auto ch = event.uChar.UnicodeChar;
#else // UNICODE
        const auto ch = event.uChar.AsciiChar;
#endif // UNICODE
        for (auto a = repeat; a-- > 0; )
        {
            OnKeyDown(event.wVirtualKeyCode, shift);

            if (ch >= 32)
            {
                OnKeyPress(ch, shift);
            }
        }
    }
    else
    {
        OnKeyUp(event.wVirtualKeyCode, shift);
    }
}

void Application::WindowBufferSizeEvent(const ::WINDOW_BUFFER_SIZE_RECORD& event)
{
    Console()->WindowBufferSizeEvent(event);
    OnResize();
}

void Application::FocusEvent(const ::FOCUS_EVENT_RECORD& event)
{
    (event.bSetFocus) ? OnActivate() : OnDeactivate();
}

void Application::Run()
{
    for (; !m_terminated; ::Sleep(1))
    {
        OnExecute();
        ProcessMessages();
    }
}

void Application::OnActivate()
{
    __noop;
}

void Application::OnDeactivate()
{
    __noop;
}

void Application::OnResize()
{
    __noop;
}

void Application::OnKeyDown(::WORD key, ::TShiftState shift)
{
    __noop;
}

void Application::OnKeyUp(::WORD key, ::TShiftState shift)
{
    __noop;
}

void Application::OnKeyPress(::_TCHAR ch, ::TShiftState shift)
{
    __noop;
}
