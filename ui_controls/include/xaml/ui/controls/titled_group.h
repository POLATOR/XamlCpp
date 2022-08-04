#ifndef XAML_UI_CONTROLS_TITLED_GROUP_H
#define XAML_UI_CONTROLS_TITLED_GROUP_H

#include <xaml/ui/container.h>

XAML_CLASS(xaml_titled_group, {0x453618c0, 0x6e20, 0x4e58, {0xa1, 0x9b, 0x2c, 0xbb, 0xf6, 0xda, 0x14, 0x3b}})

#define XAML_TITLED_GROUP_VTBL(type)                                                                                                                           \
    XAML_VTBL_INHERIT(XAML_CONTAINER_VTBL(type));                                                                                                              \
    XAML_PROP(header, type, xaml_string **, xaml_string *);                                                                                                    \
    XAML_PROP(header_halignment, type, xaml_halignment *, xaml_halignment)

XAML_DECL_INTERFACE_(xaml_titled_group, xaml_container)
{
    XAML_DECL_VTBL(xaml_titled_group, XAML_TITLED_GROUP_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_titled_group_new(xaml_titled_group **) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_titled_group_members(xaml_type_info_registration *) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_titled_group_register(xaml_meta_context *) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_LABEL_H
