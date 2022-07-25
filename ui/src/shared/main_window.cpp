#include "window.hpp"
#include <xaml/ui/main_window.h>


struct xaml_main_window_impl :  xaml_window_implement<xaml_main_window_impl, xaml_window_internal, xaml_main_window>{
	XAML_PROP_PTR_IMPL(view_model, xaml_dynamic_object);
};


EXTERN_C xaml_result XAML_CALL xaml_main_window_new(xaml_main_window**ptr) XAML_NOEXCEPT {
	return xaml_object_init<xaml_main_window_impl>(ptr);
}
EXTERN_C xaml_result XAML_CALL xaml_main_window_members(xaml_type_info_registration* __info) XAML_NOEXCEPT {
    using self_type = xaml_main_window;
    XAML_RETURN_IF_FAILED(xaml_container_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_main_window_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(title, xaml_string);
    XAML_TYPE_INFO_ADD_PROP_EVENT(location, xaml_point);
    XAML_TYPE_INFO_ADD_PROP(x, double);
    XAML_TYPE_INFO_ADD_PROP(y, double); 
    XAML_TYPE_INFO_ADD_EVENT(closing);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_resizable, bool);
    XAML_TYPE_INFO_ADD_PROP(menu_bar, xaml_control);
    XAML_TYPE_INFO_ADD_PROP(view_model, xaml_dynamic_object);
    return XAML_S_OK;
}
EXTERN_C xaml_result XAML_CALL xaml_main_window_register(xaml_meta_context* ctx) XAML_NOEXCEPT {
    XAML_TYPE_INFO_NEW(xaml_main_window, "xaml/ui/main_window.h");
    XAML_RETURN_IF_FAILED(xaml_main_window_members(__info));
    return ctx->add_type(__info);
}
