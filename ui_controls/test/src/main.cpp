#include <stdlib.h>
#include <test_window.h>
#include <xaml/result_posix.h>
#include <xaml/ui/application.h>

static xaml_test_window* wnd;

static void destroy_all(void)
{
    wnd->release();
}

xaml_result XAML_CALL xaml_main(xaml_application* app) noexcept
{
    (void)app;
    xaml_result hr;
    // Construct a window.
    XAML_GOTO_IF_FAILED(xaml_test_window_new(&wnd), exit);
    // Register destroyer.
    XAML_GOTO_IF_POSIX_ERROR(atexit(destroy_all), destroy_and_exit);
    // Show the window.
    hr = wnd->show();
exit:
    return hr;
destroy_and_exit:
    destroy_all();
    return hr;
}
