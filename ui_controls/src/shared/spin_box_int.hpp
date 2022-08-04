#ifndef XAML_UI_CONTROLS_SHARED_SPIN_BOX_INT_HPP
#define XAML_UI_CONTROLS_SHARED_SPIN_BOX_INT_HPP

#include <atomic>
#include <shared/spin_box_base.hpp>
#include <xaml/ui/controls/spin_box_int.h>

struct xaml_spin_box_int_internal : xaml_spin_box_base_internal
{
    xaml_result XAML_CALL draw(xaml_rectangle const &) noexcept override;

    xaml_result XAML_CALL draw_value() noexcept override;
    xaml_result XAML_CALL draw_min() noexcept override;
    xaml_result XAML_CALL draw_max() noexcept override;
    xaml_result XAML_CALL draw_step() noexcept override;
    xaml_result XAML_CALL draw_topHeader() noexcept override;

    xaml_result XAML_CALL init() noexcept override;
};

template <typename T, typename Internal, typename Base>
struct xaml_spin_box_int_implement : xaml_spin_box_base_implement<T, Internal, Base>
{
};

struct xaml_spin_box_int_impl : xaml_spin_box_int_implement<xaml_spin_box_int_impl, xaml_spin_box_int_internal, xaml_spin_box_int>
{
};

#endif // !XAML_UI_CONTROLS_SHARED_SPIN_BOX_INT_HPP
