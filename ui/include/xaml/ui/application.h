#ifndef XAML_UI_APPLICATION_H
#define XAML_UI_APPLICATION_H

#include <xaml/event.h>
#include <xaml/object.h>
#include <xaml/string.h>
#include <xaml/ui/window.h>
#include <xaml/vector.h>
#include <functional>
typedef enum xaml_application_theme
{
    xaml_application_theme_light,
    xaml_application_theme_dark
} xaml_application_theme;

#ifndef xaml_enumerator_1__xaml_string_defined
    #define xaml_enumerator_1__xaml_string_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_enumerator_1__xaml_string_defined

#ifndef xaml_vector_view_1__xaml_string_defined
    #define xaml_vector_view_1__xaml_string_defined
XAML_VECTOR_VIEW_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_vector_view_1__xaml_string_defined

#ifndef xaml_delegate_2__xaml_object__xaml_event_args_defined
    #define xaml_delegate_2__xaml_object__xaml_event_args_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_O(xaml_event_args))
#endif // !xaml_delegate_2__xaml_object__xaml_event_args_defined

XAML_CLASS(xaml_application, { 0x842f00fd, 0xf03c, 0x4c05, { 0xb8, 0x5f, 0x18, 0x66, 0x54, 0xa3, 0x30, 0x12 } })

#define XAML_APPLICATION_VTBL(type)                                           \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                                \
    XAML_METHOD(run, type, int*);                                             \
    XAML_METHOD(quit, type, int);                                             \
    XAML_METHOD(invoke_in_gui_thread, type, std::function<void()>);              \
    XAML_METHOD(get_cmd_lines, type, XAML_VECTOR_VIEW_1_NAME(xaml_string)**); \
    XAML_METHOD(get_main_window, type, xaml_window**);                        \
    XAML_METHOD(get_theme, type, xaml_application_theme*);                    \
    XAML_METHOD(window_added, type, xaml_window*);                            \
    XAML_METHOD(window_removed, type, xaml_window*);                          \
    XAML_EVENT(activate, type, xaml_object, xaml_event_args)

XAML_DECL_INTERFACE_(xaml_application, xaml_object)
{
    XAML_DECL_VTBL(xaml_application, XAML_APPLICATION_VTBL);
};

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_application_init(xaml_application**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_application_init_with_args(int, char**, xaml_application**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_application_current(xaml_application**) XAML_NOEXCEPT;

EXTERN_C xaml_result XAML_CALL xaml_main(xaml_application*) XAML_NOEXCEPT;

#endif // !XAML_UI_APPLICATION_H
