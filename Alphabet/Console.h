#pragma once

#include <cstdint>

#include <Windows.h>

class ConsoleRect : public SMALL_RECT
{
public:
    ~ConsoleRect() = default;

    const ::_COORD& LeftTop() const;
    const ::_COORD& RightBottom() const;

    uint16_t Width() const;
    uint16_t Height() const;
    uint32_t Size() const;

    void SetBounds(int16_t ALeft, int16_t ATop, uint16_t AWidth, uint16_t AHeight);
    void Resize(uint16_t AWidth, uint16_t AHeight);
};

class Console
{
    friend class Application;
public:
    Console(bool fast);
    ~Console() = default;

    ::HANDLE InputHandle() const;
    ::HANDLE OutputHandle() const;

    const ::ConsoleRect& ClientRect() const;

    void ClearScreen();
    void GoToXY(int16_t x, int16_t y);
    void ShowCaret(bool Value);
private:
    void WindowBufferSizeEvent(const ::WINDOW_BUFFER_SIZE_RECORD& event);
private:
    ::HANDLE m_hInput = ::GetStdHandle(STD_INPUT_HANDLE);
    ::HANDLE m_hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    ::ConsoleRect m_clientRect = { 0, 0, 0, 0 };
    ::WORD m_attributes = { 0 };
};
