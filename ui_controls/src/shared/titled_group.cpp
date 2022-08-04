#include <QResizeEvent>
#include <shared/titled_group.hpp>
#include <shared/atomic_guard.hpp>

using namespace std;

xaml_result xaml_titled_group_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_container_internal::init());

    return XAML_S_OK;
}

void xaml_titled_group_internal::on_resize_event(QResizeEvent * event) noexcept
{
    xaml_atomic_guard guard{m_resizing};
    if (!guard.test_and_set()) {
        XAML_ASSERT_SUCCEEDED(set_size(xaml_from_native(event->size())));
    }
}

xaml_result XAML_CALL xaml_titled_group_new(xaml_titled_group ** ptr) noexcept
{
    return xaml_object_init<xaml_titled_group_impl>(ptr);
}

xaml_result XAML_CALL xaml_titled_group_members(xaml_type_info_registration * __info) noexcept
{
    using self_type = xaml_titled_group;
    XAML_RETURN_IF_FAILED(xaml_container_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_titled_group_new);
    XAML_TYPE_INFO_ADD_PROP(header, xaml_string);
    XAML_TYPE_INFO_ADD_PROP(header_halignment, xaml_halignment);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_titled_group_register(xaml_meta_context * ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_titled_group, "xaml/ui/controls/titled_group.h");
    XAML_RETURN_IF_FAILED(xaml_titled_group_members(__info));
    return ctx->add_type(__info);
}
