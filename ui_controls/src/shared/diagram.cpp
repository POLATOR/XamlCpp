#include <shared/diagram.hpp>
#include <xaml/ui/controls/diagram.h>

using namespace std;

xaml_result xaml_diagram_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());

    int32_t token = 0;

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_x_scale_label_changed));
    XAML_RETURN_IF_FAILED((m_x_scale_label_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setXLabel());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_x_scale_range_changed));
    XAML_RETURN_IF_FAILED((m_x_scale_range_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setXRange());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_y_scale_label_changed));
    XAML_RETURN_IF_FAILED((m_y_scale_label_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setYLabel());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_y_scale_range_changed));
    XAML_RETURN_IF_FAILED((m_y_scale_range_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setYRange());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_curve_color_changed));
    XAML_RETURN_IF_FAILED((m_curve_color_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setCurveColor());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_curve_title_changed));
    XAML_RETURN_IF_FAILED((m_curve_title_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setCurveTitle());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_curve_data_changed));
    XAML_RETURN_IF_FAILED((m_curve_data_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(addCurveData());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_diagram_new(xaml_diagram ** ptr) noexcept
{
    return xaml_object_init<xaml_diagram_impl>(ptr);
}

xaml_result XAML_CALL xaml_diagram_members(xaml_type_info_registration * __info) noexcept
{
    using self_type = xaml_diagram;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_diagram_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(x_scale_label, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(x_scale_range, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(y_scale_label, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(y_scale_range, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(curve_color, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(curve_title, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(curve_data, xaml_string);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_diagram_register(xaml_meta_context * ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_diagram, "xaml/ui/controls/diagram.h");
    XAML_RETURN_IF_FAILED(xaml_diagram_members(__info));
    return ctx->add_type(__info);
}
