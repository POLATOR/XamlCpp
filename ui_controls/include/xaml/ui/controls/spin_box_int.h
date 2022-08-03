#ifndef XAML_UI_CONTROLS_SPIN_BOX_INT_H
#define XAML_UI_CONTROLS_SPIN_BOX_INT_H

#include <xaml/ui/controls/spin_box_base.h>

XAML_CLASS(xaml_spin_box_int, {0xee65becf, 0x3358, 0x403b, {0xbe, 0xff, 0xd4, 0xc6, 0x67, 0xa4, 0xb5, 0x2c}})

#define XAML_SPIN_BOX_INT_VTBL(type) XAML_VTBL_INHERIT(XAML_SPIN_BOX_BASE_VTBL(type))

XAML_DECL_INTERFACE_(xaml_spin_box_int, xaml_spin_box_base)
{
    XAML_DECL_VTBL(xaml_spin_box_int, XAML_SPIN_BOX_INT_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_spin_box_int_new(xaml_spin_box_int **) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_spin_box_int_members(xaml_type_info_registration *) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_spin_box_int_register(xaml_meta_context *) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_SPIN_BOX_INT_H
