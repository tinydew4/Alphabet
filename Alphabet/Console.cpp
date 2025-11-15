#include "Console.h"

#include <iostream>

const ::_COORD& ConsoleRect::LeftTop() const
{
    return *reinterpret_cast<const ::_COORD*>(&Left);
}

const ::_COORD& ConsoleRect::RightBottom() const
{
    return *reinterpret_cast<const ::_COORD*>(&Right);
}

uint16_t ConsoleRect::Width() const
{
    return Right - Left + 1;
}

uint16_t ConsoleRect::Height() const
{
    return Bottom - Top + 1;
}

uint32_t ConsoleRect::Size() const
{
    return static_cast<uint32_t>(Width()) * Height();
}

void ConsoleRect::SetBounds(int16_t ALeft, int16_t ATop, uint16_t AWidth, uint16_t AHeight)
{
    Left = ALeft;
    Top = ATop;
    Resize(AWidth, AHeight);
}

void ConsoleRect::Resize(uint16_t AWidth, uint16_t AHeight)
{
    Right = AWidth + Left - 1;
    Bottom = AHeight + Top - 1;
}

Console::Console(bool fast)
{
    ::CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (::GetConsoleScreenBufferInfo(OutputHandle(), &csbi))
    {
        m_clientRect.Resize(csbi.dwSize.X, csbi.dwSize.Y);
        m_attributes = csbi.wAttributes;
    }

    if (fast)
    {
        ::std::ios::sync_with_stdio(false);
        ::std::cin.tie(nullptr);
    }
}

::HANDLE Console::InputHandle() const
{
    return m_hInput;
}

::HANDLE Console::OutputHandle() const
{
    return m_hOutput;
}

const ::ConsoleRect& Console::ClientRect() const
{
    return m_clientRect;
}

void Console::ClearScreen()
{
    ::DWORD count;
    auto homeCoords = ClientRect().LeftTop();
    auto nLength = ClientRect().Size();
    ::FillConsoleOutputCharacter(OutputHandle(), ' ', nLength, homeCoords, &count);
    ::FillConsoleOutputAttribute(OutputHandle(), m_attributes, nLength, homeCoords, &count);
    ::SetConsoleCursorPosition(OutputHandle(), homeCoords);
}

void Console::GoToXY(int16_t x, int16_t y)
{
    ::SetConsoleCursorPosition(OutputHandle(), { x, y });
}

void Console::ShowCaret(bool Value)
{
    ::CONSOLE_CURSOR_INFO cci;
    if (::GetConsoleCursorInfo(OutputHandle(), &cci))
    {
        cci.bVisible = (Value) ? TRUE : FALSE;
        ::SetConsoleCursorInfo(OutputHandle(), &cci);
    }
}

void Console::WindowBufferSizeEvent(const ::WINDOW_BUFFER_SIZE_RECORD& event)
{
    m_clientRect.Resize(event.dwSize.X, event.dwSize.Y);
}
