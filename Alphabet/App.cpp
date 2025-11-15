#include "App.h"

#include <iostream>
#include <iomanip>

inline bool operator ==(const ::SMALL_RECT& _Left, const ::SMALL_RECT& _Right)
{
    return (*reinterpret_cast<const int64_t*>(&_Left) == *reinterpret_cast<const int64_t*>(&_Right));
}

App::App()
{
    Console()->ShowCaret(false);
    Console()->ClearScreen();
    Draw(GetDisplay(m_game));
}

App::~App()
{
    Console()->ClearScreen();
    Console()->ShowCaret(true);
}

::std::vector<::tstring> App::GetDisplay(const ::Game& game)
{
    auto target = game.CurrentTarget();
    auto started = game.Started();
    auto sequenceDone = game.SequenceDone();
    auto elapsed = game.Elapsed();

    std::vector<tstring> lines;

    _TCHAR upper = (target >= 'a' && target <= 'z')
        ? static_cast<_TCHAR>(target - 'a' + 'A')
        : target;
    lines.emplace_back(::std::format(_T("Input: {}"), upper));
    otstringstream oss;
    oss << _T("Elapsed: ");
    if (!started)
    {
        oss << _T("0.000(s)");
    }
    else
    {
        oss << std::fixed << std::setprecision(3) << elapsed.count() << _T("(s)");
    }
    lines.push_back(oss.str());
    lines.emplace_back(_T(""));
    lines.emplace_back(_T("ESC: Exit"));

    return lines;
}

::SMALL_RECT App::GetDisplayRect(const ::std::vector<::tstring>& lines)
{
    auto to_coord = [](size_t value) {
        return (::std::in_range<int16_t>(value)) ? static_cast<int16_t>(value) : (::std::numeric_limits<int16_t>::max)();
        };

    auto conWidth = to_coord(Console()->ClientRect().Width());
    auto conHeight = to_coord(Console()->ClientRect().Height());
    int16_t width = 0;
    int16_t height = to_coord(lines.size());
    for (const auto& line : lines)
    {
        auto len = to_coord(line.size());
        if (len > width)
        {
            width = len;
        }
    }

    ::SMALL_RECT rect = {
        Console()->ClientRect().Left + (conWidth > width) ? (conWidth - width) >> 1 : 0,
        Console()->ClientRect().Top + (conHeight > height) ? (conHeight - height) >> 1 : 0,
    };
    --rect.Top; // Prevent scroll on resize
    rect.Right = width + rect.Left - 1;
    rect.Bottom = height + rect.Top - 1;

    return rect;
}

void App::Draw(::std::vector<::tstring> lines)
{
    auto rect = GetDisplayRect(lines);
    auto fullRedraw = (!m_prevContent || m_prevContent->Rect != rect);
    auto width = rect.Right - rect.Left + 1;
    auto height = rect.Bottom - rect.Top + 1;

    for (auto a = height, i = 0; a-- > 0; ++i)
    {
        ::tstring padded = lines[i];
        if (::std::cmp_less(padded.size(), width))
        {
            padded.resize(width, ' ');
        }

        bool needUpdate = true;

        if (!fullRedraw && m_prevContent)
        {
            ::tstring prev = (::std::cmp_less(i, m_prevContent->Lines.size()))
                ? m_prevContent->Lines[i]
                : ::tstring();
            needUpdate = (prev != padded);
        }

        if (needUpdate)
        {
            Console()->GoToXY(rect.Left, rect.Top + i);
            tcout << padded;
        }

        lines[i] = padded;
    }

    m_prevContent = { .Rect = rect, .Lines = ::std::move(lines) };
}

void App::OnResize()
{
    _Mybase::OnResize();

    Console()->ClearScreen();
    Draw(GetDisplay(m_game));
}

void App::OnKeyDown(::WORD key, ::TShiftState shift)
{
    _Mybase::OnKeyDown(key, shift);

    switch (key)
    {
    case VK_ESCAPE:
        Terminate();
        break;
    }
}

void App::OnKeyPress(::_TCHAR ch, ::TShiftState shift)
{
    _Mybase::OnKeyPress(ch, shift);

    m_game.Input(ch, shift);
}

void App::OnExecute()
{
    m_game.Update();
    Draw(GetDisplay(m_game));
}
