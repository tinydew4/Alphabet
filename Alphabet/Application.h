#pragma once

#include <cstdint>
#include <memory>

#include <tchar.h>
#include <Windows.h>

#include "Console.h"

using TShiftState = uint8_t;
enum : TShiftState
{
    ssShift = 0x01,
    ssCtrl = 0x02,
    ssAlt = 0x04,
};

class Application
{
public:
    virtual ~Application() = default;

    const ::std::unique_ptr<::Console>& Console();
    const ::SMALL_RECT& ClientRect() const;

    virtual void Terminate();
    virtual void ProcessMessages();
    void Run();

    virtual void OnActivate();
    virtual void OnDeactivate();
    virtual void OnResize();
    virtual void OnKeyDown(::WORD key, ::TShiftState shift);
    virtual void OnKeyUp(::WORD key, ::TShiftState shift);
    virtual void OnKeyPress(::_TCHAR ch, ::TShiftState shift);
    virtual void OnExecute() = 0;
private:
    void ProcessMessage();
    virtual void KeyEvent(const ::KEY_EVENT_RECORD& event);
    virtual void WindowBufferSizeEvent(const ::WINDOW_BUFFER_SIZE_RECORD& event);
    virtual void FocusEvent(const ::FOCUS_EVENT_RECORD& event);
private:
    ::std::unique_ptr<::Console> m_console = ::std::make_unique<::Console>(true);
    ::std::atomic_bool m_terminated = { false };
    ::SMALL_RECT m_clientRect = {};
};

::TShiftState GetShiftState(::DWORD ctrlState);
