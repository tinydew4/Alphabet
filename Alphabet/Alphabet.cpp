#include "App.h"

#include <iostream>

int main()
{
    auto app = ::std::make_unique<App>();
    try
    {
        app->Run();
    }
    catch (::std::exception& e)
    {
        ::std::cerr << typeid(e).name() << " : " << e.what() << '\n';
    }
    catch (...)
    {
        try
        {
            throw ::std::exception("unknown exception");
        }
        catch (::std::exception& e)
        {
            ::std::cerr << typeid(e).name() << " : " << e.what() << '\n';
        }
    }
    return 0;
}
