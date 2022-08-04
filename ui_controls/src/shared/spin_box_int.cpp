#include <shared/atomic_guard.hpp>
#include <shared/spin_box_int.hpp>

using namespace std;

xaml_result XAML_CALL xaml_spin_box_int_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_spin_box_base_internal::init());

    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_spin_box_int_new(xaml_spin_box_int ** ptr) noexcept
{
    return xaml_object_init<xaml_spin_box_int_impl>(ptr);
}

xaml_result XAML_CALL xaml_spin_box_int_members(xaml_type_info_registration * __info) noexcept
{
    using self_type = xaml_spin_box_int;
    XAML_RETURN_IF_FAILED(xaml_spin_box_base_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_spin_box_int_new);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_spin_box_int_register(xaml_meta_context * ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_spin_box_int, "xaml/ui/controls/spin_box_int.h");
    XAML_RETURN_IF_FAILED(xaml_spin_box_int_members(__info));
    return ctx->add_type(__info);
}
