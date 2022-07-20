#include <shared/diagram.hpp>
#include <xaml/ui/controls/diagram.h>

using namespace std;

xaml_result xaml_diagram_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());

    int32_t token = 0;

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_xScaleLabel_changed));
    XAML_RETURN_IF_FAILED((m_xScaleLabel_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setXLabel());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_xScaleRange_changed));
    XAML_RETURN_IF_FAILED((m_xScaleRange_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setXRange());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_yScaleLabel_changed));
    XAML_RETURN_IF_FAILED((m_yScaleLabel_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setYLabel());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_yScaleRange_changed));
    XAML_RETURN_IF_FAILED((m_yScaleRange_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setYRange());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_curveColor_changed));
    XAML_RETURN_IF_FAILED((m_curveColor_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setCurveColor());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_curveTitle_changed));
    XAML_RETURN_IF_FAILED((m_curveTitle_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(setCurveTitle());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_curveData_changed));
    XAML_RETURN_IF_FAILED((m_curveData_changed->add(
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
    XAML_TYPE_INFO_ADD_PROP_EVENT(xScaleLabel, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(xScaleRange, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(yScaleLabel, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(yScaleRange, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(curveColor, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(curveTitle, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(curveData, xaml_string);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_diagram_register(xaml_meta_context * ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_diagram, "xaml/ui/controls/diagram.h");
    XAML_RETURN_IF_FAILED(xaml_diagram_members(__info));
    return ctx->add_type(__info);
}
