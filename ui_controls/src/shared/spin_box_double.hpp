#ifndef XAML_UI_CONTROLS_SHARED_SPIN_BOX_DOUBLE_HPP
#define XAML_UI_CONTROLS_SHARED_SPIN_BOX_DOUBLE_HPP

#include <atomic>
#include <shared/spin_box_base.hpp>
#include <xaml/ui/controls/spin_box_double.h>

struct xaml_spin_box_double_internal : xaml_spin_box_base_internal
{
    XAML_EVENT_IMPL(decimals_changed, xaml_object, xaml_string)
    XAML_PROP_STRING_EVENT_IMPL(decimals)

    xaml_result XAML_CALL draw(xaml_rectangle const &) noexcept override;

    xaml_result XAML_CALL draw_value() noexcept override;
    xaml_result XAML_CALL draw_min() noexcept override;
    xaml_result XAML_CALL draw_max() noexcept override;
    xaml_result XAML_CALL draw_step() noexcept override;
    xaml_result XAML_CALL draw_top_header() noexcept override;

    xaml_result XAML_CALL draw_decimals() noexcept;

    xaml_result XAML_CALL init() noexcept override;
};

template <typename T, typename Internal, typename Base>
struct xaml_spin_box_double_implement : xaml_spin_box_base_implement<T, Internal, Base>
{
    XAML_EVENT_INTERNAL_IMPL(decimals_changed, xaml_object, xaml_string)
    XAML_PROP_PTR_INTERNAL_IMPL(decimals, xaml_string)
};

struct xaml_spin_box_double_impl : xaml_spin_box_double_implement<xaml_spin_box_double_impl, xaml_spin_box_double_internal, xaml_spin_box_double>
{
};

#endif // !XAML_UI_CONTROLS_SHARED_SPIN_BOX_DOUBLE_HPP
