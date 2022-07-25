#ifndef XAML_UI_CONTROLS_SHARED_DIAGRAM_HPP
#define XAML_UI_CONTROLS_SHARED_DIAGRAM_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/diagram.h>

struct xaml_diagram_internal : xaml_control_internal
{
    XAML_EVENT_IMPL(xScaleLabel_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(xScaleLabel)

    XAML_EVENT_IMPL(xScaleRange_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(xScaleRange)

    XAML_EVENT_IMPL(yScaleLabel_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(yScaleLabel)

    XAML_EVENT_IMPL(yScaleRange_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(yScaleRange)

    XAML_EVENT_IMPL(curveColor_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(curveColor)

    XAML_EVENT_IMPL(curveTitle_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(curveTitle)

    XAML_EVENT_IMPL(curveData_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(curveData)

    xaml_result XAML_CALL get_reset(bool * value) noexcept
    {
        *value = false;
        return XAML_S_OK;
    }
    xaml_result XAML_CALL set_reset(bool value) noexcept
    {
        if (value) {
            return reset();
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL draw(xaml_rectangle const &) noexcept override;

    virtual xaml_result XAML_CALL setXLabel() noexcept;
    virtual xaml_result XAML_CALL setXRange() noexcept;
    virtual xaml_result XAML_CALL setYLabel() noexcept;
    virtual xaml_result XAML_CALL setYRange() noexcept;
    virtual xaml_result XAML_CALL addCurveData() noexcept;
    virtual xaml_result XAML_CALL setCurveColor() noexcept;
    virtual xaml_result XAML_CALL setCurveTitle() noexcept;
    virtual xaml_result XAML_CALL reset() noexcept;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_diagram_impl : xaml_control_implement<xaml_diagram_impl, xaml_diagram_internal, xaml_diagram>
{
    XAML_EVENT_INTERNAL_IMPL(xScaleLabel_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(xScaleLabel, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(xScaleRange_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(xScaleRange, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(yScaleLabel_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(yScaleLabel, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(yScaleRange_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(yScaleRange, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(curveColor_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(curveColor, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(curveTitle_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(curveTitle, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(curveData_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(curveData, xaml_string)

    XAML_PROP_INTERNAL_IMPL(reset, bool *, bool)
};

#endif // !XAML_UI_CONTROLS_SHARED_DIAGRAM_HPP
