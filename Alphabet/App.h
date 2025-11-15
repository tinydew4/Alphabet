#pragma once

#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include <tchar.h>

#include "Application.h"
#include "Game.h"

using tstring = ::std::basic_string<_TCHAR>;
using otstringstream = ::std::basic_ostringstream<_TCHAR>;
#ifdef UNICODE
#   define tcout (::std::wcout)
#else // UNICODE
#   define tcout (::std::cout)
#endif // UNICODE

struct Content
{
    ::SMALL_RECT Rect;
    ::std::vector<::tstring> Lines;
};

class App : public Application
{
    using _Mybase = Application;
public:
    App();
    ~App() override;

    ::std::vector<::tstring> GetDisplay(const ::Game& game);
    ::SMALL_RECT GetDisplayRect(const ::std::vector<::tstring>& lines);
    void Draw(::std::vector<::tstring> lines);

    void OnResize() override;
    void OnKeyDown(::WORD key, ::TShiftState shift) override;
    void OnKeyPress(::_TCHAR ch, ::TShiftState shift) override;
    void OnExecute() override;
private:
    ::std::optional<::Content> m_prevContent = ::std::nullopt;
    Game m_game;
};
