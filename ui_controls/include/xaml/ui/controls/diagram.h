#ifndef XAML_UI_CONTROLS_DIAGRAM_H
#define XAML_UI_CONTROLS_DIAGRAM_H

#include <xaml/ui/control.h>

XAML_CLASS(xaml_diagram, {0xcb87a70f, 0x121a, 0x40e8, {0x88, 0x60, 0x8b, 0x26, 0xaf, 0x6a, 0x38, 0xba}})

#define XAML_DIAGRAM_VTBL(type)                                                                                                                                \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));                                                                                                                \
    XAML_PROP(xScaleLabel, type, xaml_string **, xaml_string *);                                                                                               \
    XAML_EVENT(xScaleLabel_changed, type, xaml_object, xaml_string);                                                                                           \
    XAML_PROP(xScaleRange, type, xaml_string **, xaml_string *);                                                                                               \
    XAML_EVENT(xScaleRange_changed, type, xaml_object, xaml_string);                                                                                           \
    XAML_PROP(yScaleLabel, type, xaml_string **, xaml_string *);                                                                                               \
    XAML_EVENT(yScaleLabel_changed, type, xaml_object, xaml_string);                                                                                           \
    XAML_PROP(yScaleRange, type, xaml_string **, xaml_string *);                                                                                               \
    XAML_EVENT(yScaleRange_changed, type, xaml_object, xaml_string);                                                                                           \
    XAML_PROP(curveColor, type, xaml_string **, xaml_string *);                                                                                                \
    XAML_EVENT(curveColor_changed, type, xaml_object, xaml_string);                                                                                            \
    XAML_PROP(curveTitle, type, xaml_string **, xaml_string *);                                                                                                \
    XAML_EVENT(curveTitle_changed, type, xaml_object, xaml_string);                                                                                            \
    XAML_PROP(curveData, type, xaml_string **, xaml_string *);                                                                                                 \
    XAML_EVENT(curveData_changed, type, xaml_object, xaml_string);                                                                                             \
    XAML_PROP(reset, type, bool *, bool)

XAML_DECL_INTERFACE_(xaml_diagram, xaml_control)
{
    XAML_DECL_VTBL(xaml_diagram, XAML_DIAGRAM_VTBL);
};

const char DIAGRAM_COMMAND_SEPARATOR = '\b';

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_diagram_new(xaml_diagram **) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_diagram_members(xaml_type_info_registration *) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_diagram_register(xaml_meta_context *) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_DIAGRAM_H
