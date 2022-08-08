#ifndef XAML_UI_CONTROLS_SHARED_DIAGRAM_HPP
#define XAML_UI_CONTROLS_SHARED_DIAGRAM_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/diagram.h>

struct xaml_diagram_curve_impl : xaml_weak_implement<xaml_diagram_curve_impl, xaml_diagram_curve>
{
    XAML_PROP_PTR_IMPL(id, xaml_string)
    XAML_PROP_PTR_IMPL(title, xaml_string)
    XAML_PROP_PTR_IMPL(color, xaml_string)

    xaml_result XAML_CALL init() noexcept;
};

struct xaml_diagram_internal : xaml_control_internal
{
    xaml_result XAML_CALL add_curve(xaml_diagram_curve *) noexcept;
    xaml_result XAML_CALL remove_curve(xaml_diagram_curve *) noexcept;

    XAML_EVENT_IMPL(x_scale_label_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(x_scale_label)

    XAML_EVENT_IMPL(x_scale_range_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(x_scale_range)

    XAML_EVENT_IMPL(y_scale_label_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(y_scale_label)

    XAML_EVENT_IMPL(y_scale_range_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(y_scale_range)

    XAML_EVENT_IMPL(curve_color_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(curve_color)

    XAML_EVENT_IMPL(curve_title_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(curve_title)

    XAML_EVENT_IMPL(curve_data_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(curve_data)

    xaml_result XAML_CALL get_reset(bool * value) noexcept
    {
        *value = false;
        return XAML_S_OK;
    }
    xaml_result XAML_CALL set_reset(bool value) noexcept
    {
        if (value) {
            return init_curves();
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL draw(xaml_rectangle const &) noexcept override;

    virtual xaml_result XAML_CALL init_curves() noexcept;
    virtual xaml_result XAML_CALL setXLabel() noexcept;
    virtual xaml_result XAML_CALL setXRange() noexcept;
    virtual xaml_result XAML_CALL setYLabel() noexcept;
    virtual xaml_result XAML_CALL setYRange() noexcept;
    virtual xaml_result XAML_CALL addCurveData() noexcept;
    virtual xaml_result XAML_CALL setCurveColor() noexcept;
    virtual xaml_result XAML_CALL setCurveTitle() noexcept;

    xaml_result XAML_CALL init() noexcept override;

protected:
    xaml_ptr<xaml_map<xaml_string, xaml_diagram_curve>> m_curves;
};

struct xaml_diagram_impl : xaml_control_implement<xaml_diagram_impl, xaml_diagram_internal, xaml_diagram>
{
    XAML_EVENT_INTERNAL_IMPL(x_scale_label_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(x_scale_label, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(x_scale_range_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(x_scale_range, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(y_scale_label_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(y_scale_label, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(y_scale_range_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(y_scale_range, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(curve_color_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(curve_color, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(curve_title_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(curve_title, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(curve_data_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(curve_data, xaml_string)

    XAML_CPROP_INTERNAL_IMPL(curve, xaml_diagram_curve *, xaml_diagram_curve *)

    XAML_PROP_INTERNAL_IMPL(reset, bool *, bool)
};

#endif // !XAML_UI_CONTROLS_SHARED_DIAGRAM_HPP
