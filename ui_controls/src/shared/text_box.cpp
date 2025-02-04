#include <shared/atomic_guard.hpp>
#include <shared/text_box.hpp>
#include <xaml/ui/controls/text_box.h>

using namespace std;

xaml_result XAML_CALL xaml_text_box_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_text_changed));
    xaml_string_new("", &m_text);
    int32_t token;
    XAML_RETURN_IF_FAILED((m_text_changed->add(
        [this](xaml_object*, xaml_string*) noexcept -> xaml_result {
            xaml_atomic_guard guard{ m_text_changing };
            if (m_handle && !guard.test_and_set())
            {
                XAML_RETURN_IF_FAILED(draw_text());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_text_box_new(xaml_text_box** ptr) noexcept
{
    return xaml_object_init<xaml_text_box_impl>(ptr);
}

xaml_result XAML_CALL xaml_text_box_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_text_box;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_text_box_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(text, xaml_string);
    XAML_TYPE_INFO_ADD_DEF_PROP(text);

    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_text_box_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_text_box, "xaml/ui/controls/text_box.h");
    XAML_RETURN_IF_FAILED(xaml_text_box_members(__info));
    return ctx->add_type(__info);
}
