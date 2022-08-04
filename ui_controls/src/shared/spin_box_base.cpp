#include <shared/atomic_guard.hpp>
#include <shared/spin_box_base.hpp>
#include <qt/qstring.hpp>

using namespace std;

void xaml_spin_box_base_internal::on_value_changed_event(QString const & text) noexcept
{
    xaml_ptr<xaml_string> str;
    XAML_ASSERT_SUCCEEDED(xaml_string_new(text, &str));
    XAML_ASSERT_SUCCEEDED(set_value(str));
}

xaml_result XAML_CALL xaml_spin_box_base_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());

    int32_t token;

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_value_changed));
    XAML_RETURN_IF_FAILED((m_value_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            xaml_atomic_guard guard{m_value_changing};
            if (m_handle && !guard.test_and_set()) {
                XAML_RETURN_IF_FAILED(draw_value());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_top_header_changed));
    XAML_RETURN_IF_FAILED((m_top_header_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(draw_top_header());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_min_changed));
    XAML_RETURN_IF_FAILED((m_min_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(draw_min());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_max_changed));
    XAML_RETURN_IF_FAILED((m_max_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(draw_max());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_step_changed));
    XAML_RETURN_IF_FAILED((m_step_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(draw_step());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_spin_box_base_members(xaml_type_info_registration * __info) noexcept
{
    using self_type = xaml_spin_box_base;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_PROP_EVENT(value, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(top_header, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(min, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(max, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(step, xaml_string);
    XAML_TYPE_INFO_ADD_DEF_PROP(value);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_spin_box_base_register(xaml_meta_context * ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_spin_box_base, "xaml/ui/controls/spin_box_base.h");
    XAML_RETURN_IF_FAILED(xaml_spin_box_base_members(__info));
    return ctx->add_type(__info);
}
