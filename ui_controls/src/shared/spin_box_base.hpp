#ifndef XAML_UI_CONTROLS_SHARED_SPIN_BOX_BASE_HPP
#define XAML_UI_CONTROLS_SHARED_SPIN_BOX_BASE_HPP

#include <atomic>
#include <shared/control.hpp>
#include <xaml/ui/controls/spin_box_base.h>

struct xaml_spin_box_base_internal : xaml_control_internal
{
    std::atomic_bool m_value_changing{false};

    XAML_EVENT_IMPL(value_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(value)

    XAML_EVENT_IMPL(top_header_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(top_header)

    XAML_EVENT_IMPL(min_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(min)

    XAML_EVENT_IMPL(max_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(max)

    XAML_EVENT_IMPL(step_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(step)

    virtual xaml_result XAML_CALL draw_value() noexcept = 0;
    virtual xaml_result XAML_CALL draw_min() noexcept = 0;
    virtual xaml_result XAML_CALL draw_max() noexcept = 0;
    virtual xaml_result XAML_CALL draw_step() noexcept = 0;
    virtual xaml_result XAML_CALL draw_top_header() noexcept = 0;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL size_to_fit() noexcept override;

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const &, LRESULT *) noexcept override;
#elif defined(XAML_UI_COCOA)
    void on_changed() noexcept;
#elif defined(XAML_UI_GTK3)
    xaml_result XAML_CALL size_to_fit() noexcept override;

    static void on_changed(GtkWidget *, xaml_spin_box_int_internal *) noexcept;
#elif defined(XAML_UI_QT)
    void on_value_changed_event(QString const &) noexcept;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override;
};

template <typename T, typename Internal, typename Base>
struct xaml_spin_box_base_implement : xaml_control_implement<T, Internal, Base>
{
    XAML_EVENT_INTERNAL_IMPL(value_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(value, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(top_header_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(top_header, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(min_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(min, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(max_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(max, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(step_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(step, xaml_string)
};

#endif // !XAML_UI_CONTROLS_SHARED_SPIN_BOX_BASE_HPP
