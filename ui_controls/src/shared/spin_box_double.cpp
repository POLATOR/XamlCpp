#include <shared/atomic_guard.hpp>
#include <shared/spin_box_double.hpp>

using namespace std;

xaml_result XAML_CALL xaml_spin_box_double_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_spin_box_base_internal::init());

    int32_t token;

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_decimals_changed));
    XAML_RETURN_IF_FAILED((m_decimals_changed->add(
        [this](xaml_object *, xaml_string *) noexcept -> xaml_result {
            if (m_handle) {
                XAML_RETURN_IF_FAILED(draw_decimals());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));

    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_spin_box_double_new(xaml_spin_box_double ** ptr) noexcept
{
    return xaml_object_init<xaml_spin_box_double_impl>(ptr);
}

xaml_result XAML_CALL xaml_spin_box_double_members(xaml_type_info_registration * __info) noexcept
{
    using self_type = xaml_spin_box_double;
    XAML_RETURN_IF_FAILED(xaml_spin_box_base_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_spin_box_double_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(decimals, xaml_string);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_spin_box_double_register(xaml_meta_context * ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_spin_box_double, "xaml/ui/controls/spin_box_double.h");
    XAML_RETURN_IF_FAILED(xaml_spin_box_double_members(__info));
    return ctx->add_type(__info);
}
