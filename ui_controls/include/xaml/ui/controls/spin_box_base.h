#ifndef XAML_UI_CONTROLS_SPIN_BOX_BASE_H
#define XAML_UI_CONTROLS_SPIN_BOX_BASE_H

#include <xaml/ui/control.h>

XAML_CLASS(xaml_spin_box_base, {0xf007e0ca, 0x1e2f, 0x4588, {0xbf, 0xde, 0x1c, 0x9a, 0xcd, 0xd6, 0x69, 0xe7}})

#define XAML_SPIN_BOX_BASE_VTBL(type)                                                                                                                          \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));                                                                                                                \
    XAML_PROP(value, type, xaml_string **, xaml_string *);                                                                                                     \
    XAML_EVENT(value_changed, type, xaml_object, xaml_string);                                                                                                 \
    XAML_PROP(min, type, xaml_string **, xaml_string *);                                                                                                       \
    XAML_EVENT(min_changed, type, xaml_object, xaml_string);                                                                                                   \
    XAML_PROP(max, type, xaml_string **, xaml_string *);                                                                                                       \
    XAML_EVENT(max_changed, type, xaml_object, xaml_string);                                                                                                   \
    XAML_PROP(step, type, xaml_string **, xaml_string *);                                                                                                      \
    XAML_EVENT(step_changed, type, xaml_object, xaml_string);                                                                                                  \
    XAML_PROP(topHeader, type, xaml_string **, xaml_string *);                                                                                                 \
    XAML_EVENT(topHeader_changed, type, xaml_object, xaml_string)

XAML_DECL_INTERFACE_(xaml_spin_box_base, xaml_control)
{
    XAML_DECL_VTBL(xaml_spin_box_base, XAML_SPIN_BOX_BASE_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_spin_box_base_members(xaml_type_info_registration *) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_spin_box_base_register(xaml_meta_context *) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_SPIN_BOX_BASE_H
