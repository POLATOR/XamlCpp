#pragma once 

#include <xaml/ui/window.h>
#include <xaml/meta/dynamic_type.h>
// {707783A5-5622-46CB-AA25-061790B977FA}
XAML_CLASS(xaml_main_window, { 0x707783a5, 0x5622, 0x46cb, { 0xaa, 0x25, 0x6, 0x17, 0x90, 0xb9, 0x77, 0xfa }})

#define XAML_MAIN_WINDOW_VTBL(type)                                          \
    XAML_VTBL_INHERIT(XAML_MAIN_WINDOW_VTBL(type));                       \
    XAML_PROP(view_model, type, xaml_dynamic_object**, xaml_dynamic_object*);


XAML_DECL_INTERFACE_(xaml_main_window, xaml_window)
{
    XAML_DECL_VTBL(xaml_main_window, XAML_MAIN_WINDOW_VTBL);

};

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_main_window_new(xaml_main_window**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_main_window_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_main_window_register(xaml_meta_context*) XAML_NOEXCEPT;

