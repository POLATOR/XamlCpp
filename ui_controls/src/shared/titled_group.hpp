#ifndef XAML_UI_CONTROLS_SHARED_TITLE_GROUP_HPP
#define XAML_UI_CONTROLS_SHARED_TITLE_GROUP_HPP

#include <shared/container.hpp>
#include <xaml/ui/controls/titled_group.h>

struct xaml_titled_group_internal : xaml_container_internal
{
    std::atomic<bool> m_resizing{false};

    XAML_PROP_PTR_IMPL(header, xaml_string)

    XAML_PROP_IMPL(header_halignment, xaml_halignment, xaml_halignment *, xaml_halignment)

    virtual xaml_result XAML_CALL draw_header() noexcept;
    virtual xaml_result XAML_CALL draw_alignment() noexcept;

    xaml_result XAML_CALL draw(xaml_rectangle const &) noexcept override;

    xaml_result XAML_CALL draw_size() noexcept override;
    xaml_result XAML_CALL draw_child() noexcept;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL size_to_fit() noexcept override;
#elif defined(XAML_UI_QT)
    void on_resize_event(QResizeEvent * event) noexcept;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL get_client_region(xaml_rectangle *) noexcept;

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_titled_group_impl : xaml_container_implement<xaml_titled_group_impl, xaml_titled_group_internal, xaml_titled_group>
{
    XAML_PROP_PTR_INTERNAL_IMPL(header, xaml_string)

    XAML_PROP_INTERNAL_IMPL(header_halignment, xaml_halignment *, xaml_halignment)
};

#endif // !XAML_UI_CONTROLS_SHARED_TITLE_GROUP_HPP
