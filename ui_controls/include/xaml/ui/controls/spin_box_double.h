#ifndef XAML_UI_CONTROLS_SPIN_BOX_DOUBLE_H
#define XAML_UI_CONTROLS_SPIN_BOX_DOUBLE_H

#include <xaml/ui/controls/spin_box_base.h>

XAML_CLASS(xaml_spin_box_double, {0x4e6c013b, 0x6dd3, 0x4b8e, {0xb2, 0x8b, 0x22, 0x39, 0x4e, 0xa9, 0x17, 0x25}})

#define XAML_SPIN_BOX_DOUBLE_VTBL(type)                                                                                                                        \
    XAML_VTBL_INHERIT(XAML_SPIN_BOX_BASE_VTBL(type))                                                                                                           \
    XAML_PROP(decimals, type, xaml_string **, xaml_string *);                                                                                                  \
    XAML_EVENT(decimals_changed, type, xaml_object, xaml_string)

XAML_DECL_INTERFACE_(xaml_spin_box_double, xaml_spin_box_base)
{
    XAML_DECL_VTBL(xaml_spin_box_double, XAML_SPIN_BOX_DOUBLE_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_spin_box_double_new(xaml_spin_box_double **) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_spin_box_double_members(xaml_type_info_registration *) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_spin_box_double_register(xaml_meta_context *) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_SPIN_BOX_DOUBLE_H
