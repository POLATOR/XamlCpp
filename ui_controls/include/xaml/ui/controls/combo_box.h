#ifndef XAML_UI_CONTROLS_COMBO_BOX_H
#define XAML_UI_CONTROLS_COMBO_BOX_H

#include <xaml/ui/controls/items_base.h>

XAML_CLASS(xaml_combo_box_item, {0xdf1a5d1d, 0xc79d, 0x4ddc, {0x83, 0xa2, 0x16, 0xb4, 0x62, 0xd8, 0xd7, 0xb5}})

#define XAML_COMBO_BOX_ITEM_VTBL(type)                                                                                                                         \
    XAML_VTBL_INHERIT(XAML_WEAK_REFERENCE_SOURCE_VTBL(type));                                                                                                  \
    XAML_PROP(text, type, xaml_string **, xaml_string *)

XAML_DECL_INTERFACE_(xaml_combo_box_item, xaml_weak_reference_source)
{
    XAML_DECL_VTBL(xaml_combo_box_item, XAML_COMBO_BOX_ITEM_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_combo_box_item_new(xaml_combo_box_item **) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_combo_box_item_members(xaml_type_info_registration *) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_combo_box_item_register(xaml_meta_context *) XAML_NOEXCEPT;

XAML_CLASS(xaml_combo_box, {0x804bf4c0, 0xff2b, 0x445d, {0x99, 0x83, 0x4b, 0xb2, 0x0d, 0x85, 0xa4, 0x09}})

#define XAML_COMBO_BOX_VTBL(type)                                                                                                                              \
    XAML_VTBL_INHERIT(XAML_ITEMS_BASE_VTBL(type));                                                                                                             \
    XAML_PROP(text, type, xaml_string **, xaml_string *);                                                                                                      \
    XAML_EVENT(text_changed, type, xaml_object, xaml_string);                                                                                                  \
    XAML_PROP(top_header, type, xaml_string **, xaml_string *);                                                                                                \
    XAML_EVENT(top_header_changed, type, xaml_object, xaml_string);                                                                                            \
    XAML_PROP(is_editable, type, bool *, bool);                                                                                                                \
    XAML_EVENT(is_editable_changed, type, xaml_object, bool);                                                                                                  \
    XAML_METHOD(get_text_items, type, XAML_VECTOR_VIEW_1_NAME(xaml_combo_box_item) **);                                                                        \
    XAML_CPROP(text_item, type, xaml_combo_box_item *, xaml_combo_box_item *)

XAML_DECL_INTERFACE_(xaml_combo_box, xaml_items_base)
{
    XAML_DECL_VTBL(xaml_combo_box, XAML_COMBO_BOX_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_combo_box_new(xaml_combo_box **) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_combo_box_members(xaml_type_info_registration *) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_combo_box_register(xaml_meta_context *) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_COMBO_BOX_H
