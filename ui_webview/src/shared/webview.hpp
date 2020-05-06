#ifndef XAML_UI_WEBVIEW_SHARED_WEBVIEW_HPP
#define XAML_UI_WEBVIEW_SHARED_WEBVIEW_HPP

#include <atomic>
#include <shared/control.hpp>
#include <xaml/ui/controls/webview.h>

struct xaml_webview_internal : xaml_control_internal
{
    std::atomic_bool m_navigating{ false };

    XAML_EVENT_IMPL(uri_changed)
    XAML_PROP_STRING_EVENT_IMPL(uri)

    XAML_EVENT_IMPL(resource_requested)

    xaml_result XAML_CALL get_can_go_forward(bool*) noexcept;
    xaml_result XAML_CALL get_can_go_back(bool*) noexcept;

    xaml_result XAML_CALL go_forward() noexcept;
    xaml_result XAML_CALL go_back() noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    virtual xaml_result XAML_CALL draw_uri() noexcept;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_webview_impl : xaml_control_implement<xaml_webview_impl, xaml_webview_internal, xaml_webview>
{
    XAML_EVENT_INTERNAL_IMPL(uri_changed)
    XAML_PROP_PTR_INTERNAL_IMPL(uri, xaml_string)
    XAML_PROP_INTERNAL_IMPL_BASE(can_go_forward, bool*)
    XAML_PROP_INTERNAL_IMPL_BASE(can_go_back, bool*)
    XAML_EVENT_INTERNAL_IMPL(resource_requested)

    xaml_result XAML_CALL go_forward() noexcept override { return m_internal.go_forward(); }
    xaml_result XAML_CALL go_back() noexcept override { return m_internal.go_back(); }
};

#define m_outer_this this

struct xaml_webview_web_request_impl : xaml_implement<xaml_webview_web_request_impl, xaml_webview_web_request, xaml_object>
{
    XAML_PROP_PTR_IMPL(method, xaml_string)
    XAML_PROP_PTR_IMPL(uri, xaml_string)
    XAML_PROP_PTR_IMPL(data, xaml_buffer)
};

struct xaml_webview_web_response_impl : xaml_implement<xaml_webview_web_response_impl, xaml_webview_web_response, xaml_object>
{
    XAML_PROP_PTR_IMPL(content_type, xaml_string)
    XAML_PROP_PTR_IMPL(data, xaml_buffer)
};

struct xaml_webview_resource_requested_args_impl : xaml_implement<xaml_webview_resource_requested_args_impl, xaml_webview_resource_requested_args, xaml_object>
{
    XAML_PROP_PTR_IMPL(request, xaml_webview_web_request)
    XAML_PROP_PTR_IMPL(response, xaml_webview_web_response)
};

#undef m_outer_this

#endif // !XAML_UI_WEBVIEW_SHARED_WEBVIEW_HPP
