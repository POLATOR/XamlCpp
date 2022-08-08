#ifndef XAML_UI_CONTROLS_DIAGRAM_H
#define XAML_UI_CONTROLS_DIAGRAM_H

#include <xaml/ui/control.h>

XAML_CLASS(xaml_diagram_curve, {0xfb9f697, 0x67d3, 0x42d4, {0x92, 0xca, 0x89, 0x12, 0xdd, 0xb9, 0xcf, 0x70}})

#define XAML_DIAGRAM_CURVE_VTBL(type)                                                                                                                          \
    XAML_VTBL_INHERIT(XAML_WEAK_REFERENCE_SOURCE_VTBL(type));                                                                                                  \
    XAML_PROP(id, type, xaml_string **, xaml_string *);                                                                                                        \
    XAML_PROP(title, type, xaml_string **, xaml_string *);                                                                                                     \
    XAML_PROP(color, type, xaml_string **, xaml_string *)

XAML_DECL_INTERFACE_(xaml_diagram_curve, xaml_weak_reference_source)
{
    XAML_DECL_VTBL(xaml_diagram_curve, XAML_DIAGRAM_CURVE_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_diagram_curve_new(xaml_diagram_curve **) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_diagram_curve_members(xaml_type_info_registration *) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_diagram_curve_register(xaml_meta_context *) XAML_NOEXCEPT;

XAML_CLASS(xaml_diagram, {0xcb87a70f, 0x121a, 0x40e8, {0x88, 0x60, 0x8b, 0x26, 0xaf, 0x6a, 0x38, 0xba}})

#define XAML_DIAGRAM_VTBL(type)                                                                                                                                \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));                                                                                                                \
    XAML_PROP(x_scale_label, type, xaml_string **, xaml_string *);                                                                                             \
    XAML_EVENT(x_scale_label_changed, type, xaml_object, xaml_string);                                                                                         \
    XAML_PROP(x_scale_range, type, xaml_string **, xaml_string *);                                                                                             \
    XAML_EVENT(x_scale_range_changed, type, xaml_object, xaml_string);                                                                                         \
    XAML_PROP(y_scale_label, type, xaml_string **, xaml_string *);                                                                                             \
    XAML_EVENT(y_scale_label_changed, type, xaml_object, xaml_string);                                                                                         \
    XAML_PROP(y_scale_range, type, xaml_string **, xaml_string *);                                                                                             \
    XAML_EVENT(y_scale_range_changed, type, xaml_object, xaml_string);                                                                                         \
    XAML_PROP(curve_color, type, xaml_string **, xaml_string *);                                                                                               \
    XAML_EVENT(curve_color_changed, type, xaml_object, xaml_string);                                                                                           \
    XAML_PROP(curve_title, type, xaml_string **, xaml_string *);                                                                                               \
    XAML_EVENT(curve_title_changed, type, xaml_object, xaml_string);                                                                                           \
    XAML_PROP(curve_data, type, xaml_string **, xaml_string *);                                                                                                \
    XAML_EVENT(curve_data_changed, type, xaml_object, xaml_string);                                                                                            \
    XAML_CPROP(curve, type, xaml_diagram_curve *, xaml_diagram_curve *);                                                                                       \
    XAML_PROP(reset, type, bool *, bool)

XAML_DECL_INTERFACE_(xaml_diagram, xaml_control)
{
    XAML_DECL_VTBL(xaml_diagram, XAML_DIAGRAM_VTBL);
};

constexpr char DIAGRAM_COMMAND_SEPARATOR = ',';

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_diagram_new(xaml_diagram **) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_diagram_members(xaml_type_info_registration *) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_diagram_register(xaml_meta_context *) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_DIAGRAM_H
