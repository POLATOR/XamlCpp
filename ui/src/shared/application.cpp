#include <xaml/ui/application.hpp>

#if defined(XAML_UI_WINDOWS) && defined(UNICODE)
#include <shellapi.h>
#include <wil/resource.h>
#endif

using namespace std;

namespace xaml
{
    static shared_ptr<application> s_current;

    XAML_API shared_ptr<application> application::init(int argc, char_t** argv)
    {
        s_current = shared_ptr<application>(new application(argc, argv));
        s_current->init_components();
        return s_current;
    }

    XAML_API shared_ptr<application> application::current() { return s_current; }

#if defined(XAML_UI_WINDOWS) && defined(UNICODE)
    XAML_API application::application(LPWSTR lpCmdLine)
    {
        int argc;
        LPWSTR* argv = CommandLineToArgvW(lpCmdLine, &argc);
        if (argv)
        {
            wil::unique_array_ptr<LPWSTR, wil::hlocal_deleter> args(argv, argc);
            for (int i = 0; i < argc; i++)
            {
                m_cmd_lines.push_back(args[i]);
            }
        }
    }

    XAML_API shared_ptr<application> application::init(LPWSTR lpCmdLine)
    {
        s_current = shared_ptr<application>(new application(lpCmdLine));
        s_current->init_components();
        return s_current;
    }
#endif
} // namespace xaml
