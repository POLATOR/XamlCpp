#include <algorithm>
#include <cmath>
#include <test_window.h>
#include <xaml/ui/application.h>
#include <xaml/ui/controls/button.h>
#include <xaml/ui/controls/canvas.h>
#include <xaml/ui/controls/combo_box.h>
#include <xaml/ui/controls/entry.h>
#include <xaml/ui/controls/grid.h>
#include <xaml/ui/controls/label.h>
#include <xaml/ui/controls/menu_item.h>
#include <xaml/ui/controls/password_entry.h>
#include <xaml/ui/controls/progress.h>
#include <xaml/ui/controls/stack_panel.h>
#include <xaml/ui/controls/text_box.h>
#include <xaml/ui/filebox.h>
#include <xaml/ui/menu_bar.h>
#include <xaml/ui/msgbox.h>
#include <xaml/ui/timer.h>

namespace colors
{
#include <xaml/ui/colors.h>
}

using namespace std;

struct xaml_test_window_impl : xaml_implement<xaml_test_window_impl, xaml_test_window>
{
    xaml_ptr<xaml_window> m_window{};
    xaml_ptr<xaml_timer> m_timer{};
    xaml_ptr<xaml_observable_vector<xaml_object>> m_combo_source{};
    int count{ 0 };

    xaml_result XAML_CALL init() noexcept;
    xaml_result XAML_CALL show() noexcept override
    {
        return m_window->show();
    }

    xaml_result XAML_CALL on_timer_tick(xaml_object*, xaml_event_args*) noexcept;
    xaml_result XAML_CALL on_button_click(xaml_object*, xaml_event_args*) noexcept;
    xaml_result XAML_CALL on_canvas_redraw(xaml_object*, xaml_drawing_context*) noexcept;
    xaml_result XAML_CALL on_window_closing(xaml_object*, xaml_box<bool>*) noexcept;
};

xaml_result xaml_test_window_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_window_new(&m_window));
    XAML_RETURN_IF_FAILED(xaml_timer_new_interval(2s, &m_timer));
    XAML_RETURN_IF_FAILED(xaml_observable_vector_new(&m_combo_source));
    {
        xaml_ptr<xaml_string> item;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("\u05e9\u05dc\u05d5\u05dd"), &item));
        XAML_RETURN_IF_FAILED(m_combo_source->append(item));
    }
    {
        xaml_ptr<xaml_string> item;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("锟斤拷"), &item));
        XAML_RETURN_IF_FAILED(m_combo_source->append(item));
    }
    {
        xaml_ptr<xaml_string> item;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("\u05e9-\u043c-\u03bd"), &item));
        XAML_RETURN_IF_FAILED(m_combo_source->append(item));
    }
    {
        xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new(xaml_mem_fn(&xaml_test_window_impl::on_timer_tick, this), &callback)));
        int32_t token;
        XAML_RETURN_IF_FAILED(m_timer->add_tick(callback, &token));
    }
    {
        xaml_ptr<xaml_delegate<xaml_object, xaml_box<bool>>> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new(xaml_mem_fn(&xaml_test_window_impl::on_window_closing, this), &callback)));
        int32_t token;
        XAML_RETURN_IF_FAILED(m_window->add_closing(callback, &token));
    }
    // Set title, location, and size.
    xaml_ptr<xaml_string> title;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Test window"), &title));
    XAML_RETURN_IF_FAILED(m_window->set_title(title));
    XAML_RETURN_IF_FAILED(m_window->set_location({ 100, 100 }));
    XAML_RETURN_IF_FAILED(m_window->set_size({ 800, 600 }));
    // Construct a grid.
    xaml_ptr<xaml_grid> g;
    XAML_RETURN_IF_FAILED(xaml_grid_new(&g));
    // Add 3 columns and 3 rows, with same width or height.
    XAML_RETURN_IF_FAILED(g->add_column({ 1, xaml_grid_layout_star }));
    XAML_RETURN_IF_FAILED(g->add_column({ 1, xaml_grid_layout_star }));
    XAML_RETURN_IF_FAILED(g->add_column({ 1, xaml_grid_layout_star }));
    XAML_RETURN_IF_FAILED(g->add_row({ 1, xaml_grid_layout_auto }));
    XAML_RETURN_IF_FAILED(g->add_row({ 1, xaml_grid_layout_auto }));
    XAML_RETURN_IF_FAILED(g->add_row({ 1, xaml_grid_layout_star }));
    XAML_RETURN_IF_FAILED(g->add_row({ 1, xaml_grid_layout_star }));
    // Set g as child of the window.
    XAML_RETURN_IF_FAILED(m_window->set_child(g));
    {
        xaml_ptr<xaml_progress> prog;
        XAML_RETURN_IF_FAILED(xaml_progress_new(&prog));
        XAML_RETURN_IF_FAILED(prog->set_margin({ 10, 10, 10, 10 }));
        XAML_RETURN_IF_FAILED(prog->set_valignment(xaml_valignment_center));
        XAML_RETURN_IF_FAILED(prog->set_minimum(0));
        XAML_RETURN_IF_FAILED(prog->set_maximum(100));
        XAML_RETURN_IF_FAILED(prog->set_value(30));
        XAML_RETURN_IF_FAILED(prog->set_is_indeterminate(true));
        XAML_RETURN_IF_FAILED(g->add_child(prog));
        XAML_RETURN_IF_FAILED(xaml_grid_set_column(prog, 2));
        XAML_RETURN_IF_FAILED(xaml_grid_set_row(prog, 2));
        // Construct a button.
        xaml_ptr<xaml_button> btn;
        XAML_RETURN_IF_FAILED(xaml_button_new(&btn));
        // Set margin and text.
        XAML_RETURN_IF_FAILED(btn->set_margin({ 10, 10, 10, 10 }));
        xaml_ptr<xaml_string> text;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello"), &text));
        XAML_RETURN_IF_FAILED(btn->set_text(text));
        // Add a handler for button.
        {
            xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
            XAML_RETURN_IF_FAILED((xaml_delegate_new(
                [prog](xaml_object*, xaml_event_args*) noexcept -> xaml_result {
                    return prog->set_is_indeterminate(false);
                },
                &callback)));
            int32_t token;
            XAML_RETURN_IF_FAILED(btn->add_click(callback, &token));
        }
        {
            xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
            XAML_RETURN_IF_FAILED((xaml_delegate_new(xaml_mem_fn(&xaml_test_window_impl::on_button_click, this), &callback)));
            int32_t token;
            XAML_RETURN_IF_FAILED(btn->add_click(callback, &token));
        }
        // Set the button to be placed in the center vertically.
        XAML_RETURN_IF_FAILED(btn->set_valignment(xaml_valignment_center));
        // Add the button to the grid, and set its column and row.
        XAML_RETURN_IF_FAILED(g->add_child(btn));
        XAML_RETURN_IF_FAILED(xaml_grid_set_column(btn, 1));
        XAML_RETURN_IF_FAILED(xaml_grid_set_row(btn, 2));
    }
    {
        // Construct a label.
        xaml_ptr<xaml_label> lb;
        XAML_RETURN_IF_FAILED(xaml_label_new(&lb));
        // Set the margin, text alignmemt, vertical alignment and text.
        lb->set_margin({ 10, 10, 10, 10 });
        lb->set_text_halignment(xaml_halignment_right);
        lb->set_valignment(xaml_valignment_center);
        xaml_ptr<xaml_string> text;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("Username:"), &text));
        lb->set_text(text);
        // Add the label to the grid, and set its column and row.
        g->add_child(lb);
        xaml_grid_set_column(lb, 0);
        xaml_grid_set_row(lb, 0);
    }
    {
        // Construct an entry (single-line text box).
        xaml_ptr<xaml_entry> et;
        XAML_RETURN_IF_FAILED(xaml_entry_new(&et));
        et->set_margin({ 0, 5, 0, 5 });
        xaml_ptr<xaml_string> text;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("John"), &text));
        et->set_text(text);
        et->set_halignment(xaml_halignment_left);
        et->set_valignment(xaml_valignment_center);
        g->add_child(et);
        xaml_grid_set_column(et, 1);
        xaml_grid_set_row(et, 0);
    }
    {
        xaml_ptr<xaml_label> lb;
        XAML_RETURN_IF_FAILED(xaml_label_new(&lb));
        // Set the margin, text alignmemt, vertical alignment and text.
        lb->set_margin({ 10, 10, 10, 10 });
        lb->set_text_halignment(xaml_halignment_right);
        lb->set_valignment(xaml_valignment_center);
        xaml_ptr<xaml_string> text;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("Password:"), &text));
        lb->set_text(text);
        // Add the label to the grid, and set its column and row.
        g->add_child(lb);
        xaml_grid_set_column(lb, 0);
        xaml_grid_set_row(lb, 1);
    }
    {
        xaml_ptr<xaml_password_entry> et;
        XAML_RETURN_IF_FAILED(xaml_password_entry_new(&et));
        et->set_margin({ 0, 5, 0, 5 });
        xaml_ptr<xaml_string> text;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("123456"), &text));
        et->set_text(text);
        et->set_halignment(xaml_halignment_stretch);
        et->set_valignment(xaml_valignment_center);
        g->add_child(et);
        xaml_grid_set_column(et, 1);
        xaml_grid_set_row(et, 1);
    }
    {
        // Construct a cnavas.
        xaml_ptr<xaml_canvas> cv;
        XAML_RETURN_IF_FAILED(xaml_canvas_new(&cv));
        // Set the margin.
        cv->set_margin({ 10, 10, 10, 10 });
        // Add a handler to the canvas to handle the redraw event.
        // You should always draw in that event.
        {
            xaml_ptr<xaml_delegate<xaml_object, xaml_drawing_context>> callback;
            XAML_RETURN_IF_FAILED((xaml_delegate_new(xaml_mem_fn(&xaml_test_window_impl::on_canvas_redraw, this), &callback)));
            int32_t token;
            XAML_RETURN_IF_FAILED(cv->add_redraw(callback, &token));
        }
        // Add the canvas to the grid, and set its column and row.
        g->add_child(cv);
        xaml_grid_set_column(cv, 0);
        xaml_grid_set_row(cv, 3);
    }
    {
        xaml_ptr<xaml_combo_box> box;
        XAML_RETURN_IF_FAILED(xaml_combo_box_new(&box));
        box->set_halignment(xaml_halignment_center);
        box->set_valignment(xaml_valignment_top);
        box->set_items(m_combo_source);
        box->set_sel_id(1);
        {
            xaml_ptr<xaml_grid> panel;
            XAML_RETURN_IF_FAILED(xaml_grid_new(&panel));
            XAML_RETURN_IF_FAILED(panel->set_margin({ 10, 10, 10, 10 }));
            XAML_RETURN_IF_FAILED(panel->add_row({ 0, xaml_grid_layout_auto }));
            XAML_RETURN_IF_FAILED(panel->add_row({ 1, xaml_grid_layout_star }));
            panel->add_child(box);
            {
                xaml_ptr<xaml_text_box> text;
                XAML_RETURN_IF_FAILED(xaml_text_box_new(&text));
                XAML_RETURN_IF_FAILED(text->set_margin({ 5, 5, 5, 5 }));
                xaml_ptr<xaml_string> text_text;
                XAML_RETURN_IF_FAILED(xaml_string_new(U("This is a long long long long long long long long long long long long long long long long long long long long long long long long long long long long long long text.\n"
                                                        "And this is the next line."),
                                                      &text_text));
                XAML_RETURN_IF_FAILED(text->set_text(text_text));
                panel->add_child(text);
                xaml_grid_set_row(text, 1);
            }
            g->add_child(panel);
            xaml_grid_set_column(panel, 1);
            xaml_grid_set_row(panel, 3);
        }
        {
            xaml_ptr<xaml_stack_panel> panel;
            XAML_RETURN_IF_FAILED(xaml_stack_panel_new(&panel));
            panel->set_orientation(xaml_orientation_vertical);
            panel->set_margin({ 5, 5, 5, 5 });
            {
                xaml_ptr<xaml_button> btn;
                XAML_RETURN_IF_FAILED(xaml_button_new(&btn));
                btn->set_margin({ 5, 5, 5, 5 });
                xaml_ptr<xaml_string> text;
                XAML_RETURN_IF_FAILED(xaml_string_new(U("Push"), &text));
                btn->set_text(text);
                xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
                XAML_RETURN_IF_FAILED((xaml_delegate_new(
                    [this](xaml_object*, xaml_event_args*) noexcept -> xaml_result {
                        xaml_ptr<xaml_string> item;
                        XAML_RETURN_IF_FAILED(xaml_string_new(U("\U0001D49E-\u043F\u0440\u0438\u0432\u0435\u0442-\u3084\u3042"), &item));
                        return m_combo_source->append(item);
                    },
                    &callback)));
                int32_t token;
                btn->add_click(callback, &token);
                panel->add_child(btn);
            }
            {
                xaml_ptr<xaml_button> btn;
                XAML_RETURN_IF_FAILED(xaml_button_new(&btn));
                btn->set_margin({ 5, 5, 5, 5 });
                xaml_ptr<xaml_string> text;
                XAML_RETURN_IF_FAILED(xaml_string_new(U("Pop"), &text));
                btn->set_text(text);
                xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
                XAML_RETURN_IF_FAILED((xaml_delegate_new(
                    [this](xaml_object*, xaml_event_args*) noexcept -> xaml_result {
                        return m_combo_source->remove_at_end();
                    },
                    &callback)));
                int32_t token;
                btn->add_click(callback, &token);
                panel->add_child(btn);
            }
            {
                xaml_ptr<xaml_button> btn;
                XAML_RETURN_IF_FAILED(xaml_button_new(&btn));
                btn->set_margin({ 5, 5, 5, 5 });
                xaml_ptr<xaml_string> text;
                XAML_RETURN_IF_FAILED(xaml_string_new(U("Show"), &text));
                btn->set_text(text);
                xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
                XAML_RETURN_IF_FAILED((xaml_delegate_new(
                    [this, box](xaml_object*, xaml_event_args*) noexcept -> xaml_result {
                        int32_t sel;
                        XAML_RETURN_IF_FAILED(box->get_sel_id(&sel));
                        int32_t size;
                        XAML_RETURN_IF_FAILED(m_combo_source->get_size(&size));
                        if (sel >= 0 && sel < size)
                        {
                            xaml_ptr<xaml_object> item;
                            XAML_RETURN_IF_FAILED(m_combo_source->get_at(sel, &item));
                            xaml_ptr<xaml_string> str;
                            XAML_RETURN_IF_FAILED(item->query(&str));
                            xaml_ptr<xaml_string> title;
                            XAML_RETURN_IF_FAILED(xaml_string_new(U("Show selected item"), &title));
                            xaml_msgbox_result res;
                            XAML_RETURN_IF_FAILED(xaml_msgbox(m_window, str, title, nullptr, xaml_msgbox_none, xaml_msgbox_buttons_ok, &res));
                        }
                        return XAML_S_OK;
                    },
                    &callback)));
                int32_t token;
                btn->add_click(callback, &token);
                panel->add_child(btn);
            }
            g->add_child(panel);
            xaml_grid_set_column(panel, 2);
            xaml_grid_set_row(panel, 3);
        }
    }
    {
        xaml_ptr<xaml_menu_bar> mbar;
        XAML_RETURN_IF_FAILED(xaml_menu_bar_new(&mbar));
        {
            xaml_ptr<xaml_popup_menu_item> mfile;
            XAML_RETURN_IF_FAILED(xaml_popup_menu_item_new(&mfile));
            {
                xaml_ptr<xaml_string> text;
                XAML_RETURN_IF_FAILED(xaml_string_new(U("File"), &text));
                XAML_RETURN_IF_FAILED(mfile->set_text(text));
            }
            {
                xaml_ptr<xaml_menu_item> mquit;
                XAML_RETURN_IF_FAILED(xaml_menu_item_new(&mquit));
                {
                    xaml_ptr<xaml_string> text;
                    XAML_RETURN_IF_FAILED(xaml_string_new(U("Close"), &text));
                    XAML_RETURN_IF_FAILED(mquit->set_text(text));
                }
                {
                    xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
                    XAML_RETURN_IF_FAILED((xaml_delegate_new(
                        [this](xaml_object*, xaml_event_args*) noexcept -> xaml_result {
                            return m_window->close();
                        },
                        &callback)));
                    int32_t token;
                    XAML_RETURN_IF_FAILED(mquit->add_click(callback, &token));
                }
                XAML_RETURN_IF_FAILED(mfile->add_submenu(mquit));
            }
            XAML_RETURN_IF_FAILED(mbar->add_child(mfile));
        }
        XAML_RETURN_IF_FAILED(m_window->set_menu_bar(mbar));
    }
    return XAML_S_OK;
}

xaml_result xaml_test_window_impl::on_timer_tick(xaml_object*, xaml_event_args*) noexcept
{
    xaml_msgbox_result res;
    {
        xaml_ptr<xaml_string> msg;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello world!"), &msg));
        xaml_ptr<xaml_string> title;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello"), &title));
        XAML_RETURN_IF_FAILED(xaml_msgbox(m_window, msg, title, nullptr, xaml_msgbox_info, xaml_msgbox_buttons_ok, &res));
    }
    if (++count >= 3)
    {
        XAML_RETURN_IF_FAILED(m_timer->stop());
        xaml_ptr<xaml_open_filebox> openbox;
        XAML_RETURN_IF_FAILED(xaml_open_filebox_new(&openbox));
        {
            xaml_ptr<xaml_string> title;
            XAML_RETURN_IF_FAILED(xaml_string_new(U("Open file"), &title));
            XAML_RETURN_IF_FAILED(openbox->set_title(title));
        }
        xaml_ptr<xaml_vector<xaml_filebox_filter>> filters;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ { U("XAML file"), U("*.xaml") } }, &filters));
        XAML_RETURN_IF_FAILED(openbox->set_filters(filters));
        if (XAML_SUCCEEDED(openbox->show(m_window)))
        {
            xaml_ptr<xaml_string> result;
            XAML_RETURN_IF_FAILED(openbox->get_result(&result));
            xaml_ptr<xaml_string> title;
            XAML_RETURN_IF_FAILED(xaml_string_new(U("Open file"), &title));
            XAML_RETURN_IF_FAILED(xaml_msgbox(m_window, result, title, nullptr, xaml_msgbox_none, xaml_msgbox_buttons_ok, &res));
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_test_window_impl::on_button_click(xaml_object* sender, xaml_event_args*) noexcept
{
    xaml_ptr<xaml_button> btn;
    XAML_RETURN_IF_FAILED(sender->query(&btn));
    xaml_ptr<xaml_string> text;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello world!"), &text));
    XAML_RETURN_IF_FAILED(btn->set_text(text));
    count = 0;
    return m_timer->start();
}

xaml_result xaml_test_window_impl::on_canvas_redraw(xaml_object* sender, xaml_drawing_context* dc) noexcept
{
    xaml_ptr<xaml_canvas> cv;
    XAML_RETURN_IF_FAILED(sender->query(&cv));
    xaml_ptr<xaml_application> current_app;
    XAML_RETURN_IF_FAILED(xaml_application_current(&current_app));
    xaml_application_theme theme;
    XAML_RETURN_IF_FAILED(current_app->get_theme(&theme));
    bool is_dark = theme == xaml_application_theme_dark;
    xaml_size csize;
    XAML_RETURN_IF_FAILED(cv->get_size(&csize));
    auto cx = csize.width / 2;
    auto cy = csize.height / 2;
    auto r = (min)(cx, cy) - 2;
    xaml_ptr<xaml_brush_pen> pen1;
    XAML_RETURN_IF_FAILED(xaml_brush_pen_new_solid(is_dark ? colors::white : colors::black, 1, &pen1));
    XAML_RETURN_IF_FAILED(dc->draw_arc(pen1, { cx - r, cy - r, r * 2, r * 2 }, M_PI, 2 * M_PI));
    XAML_RETURN_IF_FAILED(dc->draw_line(pen1, { cx - r, cy }, { cx + r, cy }));
    xaml_ptr<xaml_linear_gradient_brush> brush2;
    XAML_RETURN_IF_FAILED(xaml_linear_gradient_brush_new(&brush2));
    XAML_RETURN_IF_FAILED(brush2->set_start_point({ 0, 0 }));
    XAML_RETURN_IF_FAILED(brush2->set_end_point({ 0, 1 }));
    XAML_RETURN_IF_FAILED(brush2->add_stop({ colors::sky_blue, 0 }));
    XAML_RETURN_IF_FAILED(brush2->add_stop({ is_dark ? colors::white : colors::black, 1 }));
    xaml_ptr<xaml_brush_pen> pen2;
    XAML_RETURN_IF_FAILED(xaml_brush_pen_new(brush2, 1, &pen2));
    XAML_RETURN_IF_FAILED(dc->draw_round_rect(pen2, { cx - r - 1, cy - r - 1, r * 2 + 2, r * 1.618 + 2 }, { r / 10, r / 10 }));
    XAML_RETURN_IF_FAILED(dc->draw_arc(pen1, { cx - r - 1 + r / 10, cy + r * 0.618 + 1 - r * 0.382 / 2, r * 2 - r / 5, r * 0.382 }, 0, M_PI));
    xaml_ptr<xaml_string> text;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello world!"), &text));
    xaml_ptr<xaml_radial_gradient_brush> brush3;
    XAML_RETURN_IF_FAILED(xaml_radial_gradient_brush_new(&brush3));
    XAML_RETURN_IF_FAILED(brush3->set_center({ 0.5, 0.5 }));
    XAML_RETURN_IF_FAILED(brush3->set_origin({ 0.2, 0.5 }));
    XAML_RETURN_IF_FAILED(brush3->add_stop({ colors::white_smoke, 0 }));
    XAML_RETURN_IF_FAILED(brush3->add_stop({ colors::pink, 1 }));
    xaml_drawing_font str_font = { U("Arial"), r / 5, false, false, xaml_halignment_center, xaml_valignment_bottom };
    XAML_RETURN_IF_FAILED(dc->draw_string(brush3, str_font, { cx, cy }, text));
    xaml_rectangle str_rect;
    XAML_RETURN_IF_FAILED(dc->measure_string(str_font, { cx, cy }, text, &str_rect));
    XAML_RETURN_IF_FAILED(dc->draw_rect(pen1, str_rect));
    return XAML_S_OK;
}

xaml_result xaml_test_window_impl::on_window_closing(xaml_object* sender, xaml_box<bool>* box) noexcept
{
    xaml_ptr<xaml_window> win;
    XAML_RETURN_IF_FAILED(sender->query(&win));
    bool handled;
    XAML_RETURN_IF_FAILED(xaml_unbox_value(box, &handled));
    xaml_ptr<xaml_string> msg;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Do you want to close this window?"), &msg));
    xaml_ptr<xaml_string> title;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Close window"), &title));
    xaml_msgbox_result res;
    XAML_RETURN_IF_FAILED(xaml_msgbox(win, msg, title, nullptr, xaml_msgbox_question, xaml_msgbox_buttons_yes_no, &res));
    handled = res != xaml_msgbox_result_yes;
    XAML_RETURN_IF_FAILED(box->set_value(handled));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_test_window_new(xaml_test_window** ptr) noexcept
{
    return xaml_object_init<xaml_test_window_impl>(ptr);
}
