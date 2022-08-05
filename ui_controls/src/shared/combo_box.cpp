#include <shared/combo_box.hpp>
#include <xaml/ui/controls/combo_box.h>

using namespace std;

xaml_result XAML_CALL xaml_combo_box_item_new(xaml_combo_box_item ** ptr) XAML_NOEXCEPT
{
    XAML_RETURN_IF_FAILED(xaml_object_init<xaml_combo_box_item_impl>(ptr));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_combo_box_item_members(xaml_type_info_registration * __info) XAML_NOEXCEPT
{
    using self_type = xaml_combo_box_item;
    XAML_RETURN_IF_FAILED(xaml_element_base_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_combo_box_item_new);
    XAML_TYPE_INFO_ADD_PROP(text, xaml_string);
    XAML_TYPE_INFO_ADD_DEF_PROP(text);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_combo_box_item_register(xaml_meta_context * ctx) XAML_NOEXCEPT
{
    XAML_TYPE_INFO_NEW(xaml_combo_box_item, "xaml/ui/controls/combo_box.h");
    XAML_RETURN_IF_FAILED(xaml_combo_box_item_members(__info));
    return ctx->add_type(__info);
}

xaml_result xaml_combo_box_item_impl::init() noexcept
{
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_combo_box_internal::add_text_item(xaml_combo_box_item * item) noexcept
{
    if (item) {
        XAML_RETURN_IF_FAILED(m_text_items->append(item));
    }
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_combo_box_internal::remove_text_item(xaml_combo_box_item * item) noexcept
{
    int32_t index;
    XAML_RETURN_IF_FAILED(m_text_items->index_of(item, &index));
    if (index >= 0) {
        XAML_RETURN_IF_FAILED(m_text_items->remove_at(index));
    }
    return XAML_S_OK;
}

xaml_result xaml_combo_box_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_items_base_internal::init());

    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_text_items));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_text_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_editable_changed));

    int32_t token = 0;

    XAML_RETURN_IF_FAILED((m_items_changed->add(
        [this](xaml_object *, xaml_observable_vector<xaml_object> *) noexcept -> xaml_result {
            if (m_handle)
                XAML_RETURN_IF_FAILED(draw_items());
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

    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_combo_box_new(xaml_combo_box ** ptr) noexcept
{
    return xaml_object_init<xaml_combo_box_impl>(ptr);
}

xaml_result XAML_CALL xaml_combo_box_members(xaml_type_info_registration * __info) noexcept
{
    using self_type = xaml_combo_box;
    XAML_RETURN_IF_FAILED(xaml_items_base_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_combo_box_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(text, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(top_header, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_editable, bool);
    XAML_TYPE_INFO_ADD_CPROP(text_item, xaml_combo_box_item);
    XAML_TYPE_INFO_ADD_DEF_PROP(text_item);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_combo_box_register(xaml_meta_context * ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_combo_box, "xaml/ui/controls/combo_box.h");
    XAML_RETURN_IF_FAILED(xaml_combo_box_members(__info));
    return ctx->add_type(__info);
}
