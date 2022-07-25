#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>
#include <iomanip>

#include <test_window.h>
#include <xaml/ui/application.h>
#include <xaml/ui/controls/button.h>
#include <xaml/ui/controls/canvas.h>
#include <xaml/ui/controls/diagram.h>
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

namespace colors {
#include <xaml/ui/colors.h>
}

using namespace std;

struct xaml_test_window_impl : xaml_implement<xaml_test_window_impl, xaml_test_window>
{
    xaml_ptr<xaml_window> m_window{};
    xaml_ptr<xaml_timer> m_timer{};
    xaml_ptr<xaml_observable_vector<xaml_object>> m_combo_source{};
    int count{0};

    xaml_result XAML_CALL init() noexcept;
    xaml_result XAML_CALL show() noexcept override
    {
        return m_window->show();
    }
};

static std::mt19937 g_generator{std::random_device{}()};
static double g_min = 5.0;
static double g_max = 5.0;

xaml_result ResetColor(xaml_diagram * diagram, const std::string & curveName)
{
    std::uniform_int_distribution<> dist(0, 0x00FFFFFF);
    std::stringstream stream;
    stream << curveName << DIAGRAM_COMMAND_SEPARATOR << "#" << std::setfill('0') << std::hex << std::setw(6) << dist(g_generator);

    xaml_ptr<xaml_string> colorData;
    XAML_RETURN_IF_FAILED(xaml_string_new(stream.str(), &colorData));
    return diagram->set_curveColor(colorData);
}

xaml_result ResetTitle(xaml_diagram * diagram, const std::string & curveName)
{
    std::uniform_int_distribution<> dist(0, 0x00FFFFFF);
    std::stringstream stream;
    stream << curveName << DIAGRAM_COMMAND_SEPARATOR << "Random title # " << dist(g_generator);

    xaml_ptr<xaml_string> titleData;
    XAML_RETURN_IF_FAILED(xaml_string_new(stream.str(), &titleData));
    return diagram->set_curveTitle(titleData);
}

xaml_result ResetMinMax(xaml_diagram * diagram)
{
    std::uniform_real_distribution<> dist(0, 10.0);
    g_min = -dist(g_generator);
    g_max = dist(g_generator);
    if (g_min > g_max) {
        std::swap(g_min, g_max);
    }
    std::stringstream stream;
    stream << g_min << DIAGRAM_COMMAND_SEPARATOR << g_max;

    xaml_ptr<xaml_string> range;
    XAML_RETURN_IF_FAILED(xaml_string_new(stream.str(), &range));
    XAML_RETURN_IF_FAILED(diagram->set_xScaleRange(range));
    return diagram->set_yScaleRange(range);
}

xaml_result CreateCurveData(xaml_diagram * diagram, const std::string & curveName)
{
    std::uniform_real_distribution<> dist(0, g_max * 2.0);

    std::stringstream stream;
    stream << curveName;
    for (int i = 0; i < 10; ++i) {
        stream << DIAGRAM_COMMAND_SEPARATOR << (dist(g_generator) - g_max);
    }
    xaml_ptr<xaml_string> curveData;
    XAML_RETURN_IF_FAILED(xaml_string_new(stream.str(), &curveData));
    return diagram->set_curveData(curveData);
}

xaml_result xaml_test_window_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_window_new(&m_window));
    // Set title, location, and size.
    xaml_ptr<xaml_string> title;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Diagram test window"), &title));
    XAML_RETURN_IF_FAILED(m_window->set_title(title));
    XAML_RETURN_IF_FAILED(m_window->set_location({100, 100}));
    XAML_RETURN_IF_FAILED(m_window->set_size({800, 600}));
    // Construct a grid.
    xaml_ptr<xaml_grid> g;
    XAML_RETURN_IF_FAILED(xaml_grid_new(&g));
    // Add 3 columns and 3 rows, with same width or height.
    XAML_RETURN_IF_FAILED(g->add_column({1, xaml_grid_layout_star}));
    XAML_RETURN_IF_FAILED(g->add_row({1, xaml_grid_layout_auto}));
    XAML_RETURN_IF_FAILED(g->add_row({1, xaml_grid_layout_star}));
    XAML_RETURN_IF_FAILED(g->set_margin({10, 10, 10, 10}));
    // Set g as child of the window.
    XAML_RETURN_IF_FAILED(m_window->set_child(g));

    xaml_ptr<xaml_diagram> diagram;
    XAML_RETURN_IF_FAILED(xaml_diagram_new(&diagram));
    xaml_ptr<xaml_string> labelText;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("X Label long long header"), &labelText));
    XAML_RETURN_IF_FAILED(diagram->set_xScaleLabel(labelText));
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Y Label short header without extrawords"), labelText.put()));
    XAML_RETURN_IF_FAILED(diagram->set_yScaleLabel(labelText));
    XAML_RETURN_IF_FAILED(g->add_child(diagram));
    XAML_RETURN_IF_FAILED(xaml_grid_set_column(diagram, 0));
    XAML_RETURN_IF_FAILED(xaml_grid_set_row(diagram, 0));

    xaml_ptr<xaml_grid> g2;
    XAML_RETURN_IF_FAILED(xaml_grid_new(&g2));
    // Add 3 columns and 3 rows, with same width or height.
    XAML_RETURN_IF_FAILED(g2->add_column({1, xaml_grid_layout_star}));
    XAML_RETURN_IF_FAILED(g2->add_column({1, xaml_grid_layout_star}));
    XAML_RETURN_IF_FAILED(g2->add_column({1, xaml_grid_layout_star}));
    XAML_RETURN_IF_FAILED(g2->add_column({1, xaml_grid_layout_star}));
    XAML_RETURN_IF_FAILED(g2->add_column({1, xaml_grid_layout_star}));
    XAML_RETURN_IF_FAILED(g2->add_row({1, xaml_grid_layout_star}));
    XAML_RETURN_IF_FAILED(g2->set_margin({10, 10, 10, 10}));
    {
        xaml_ptr<xaml_button> btn;
        XAML_RETURN_IF_FAILED(xaml_button_new(&btn));
        xaml_ptr<xaml_string> text;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("Init"), &text));
        XAML_RETURN_IF_FAILED(btn->set_text(text));
        XAML_RETURN_IF_FAILED(btn->set_halignment(xaml_halignment_center));
        XAML_RETURN_IF_FAILED(btn->set_valignment(xaml_valignment_bottom));

        xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new(
            [diagram](xaml_object *, xaml_event_args *) noexcept -> xaml_result {
                return ResetMinMax(diagram);
                return XAML_S_OK;
            },
            &callback)));
        int32_t token;
        XAML_RETURN_IF_FAILED(btn->add_click(callback, &token));
        XAML_RETURN_IF_FAILED(g2->add_child(btn));
        XAML_RETURN_IF_FAILED(xaml_grid_set_column(btn, 0));
        XAML_RETURN_IF_FAILED(xaml_grid_set_row(btn, 0));
    }
    {
        xaml_ptr<xaml_button> btn;
        XAML_RETURN_IF_FAILED(xaml_button_new(&btn));
        xaml_ptr<xaml_string> text;
        XAML_RETURN_IF_FAILED(xaml_string_new("Colors", &text));
        XAML_RETURN_IF_FAILED(btn->set_text(text));
        XAML_RETURN_IF_FAILED(btn->set_halignment(xaml_halignment_center));
        XAML_RETURN_IF_FAILED(btn->set_valignment(xaml_valignment_bottom));

        xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new(
            [diagram](xaml_object *, xaml_event_args *) noexcept -> xaml_result {
                XAML_RETURN_IF_FAILED(ResetColor(diagram, "Curve #1"));
                XAML_RETURN_IF_FAILED(ResetColor(diagram, "Curve #2"));
                return XAML_S_OK;
            },
            &callback)));
        int32_t token;
        XAML_RETURN_IF_FAILED(btn->add_click(callback, &token));
        XAML_RETURN_IF_FAILED(g2->add_child(btn));
        XAML_RETURN_IF_FAILED(xaml_grid_set_column(btn, 1));
        XAML_RETURN_IF_FAILED(xaml_grid_set_row(btn, 0));
    }
    {
        xaml_ptr<xaml_button> btn;
        XAML_RETURN_IF_FAILED(xaml_button_new(&btn));
        xaml_ptr<xaml_string> text;
        XAML_RETURN_IF_FAILED(xaml_string_new("Titles", &text));
        XAML_RETURN_IF_FAILED(btn->set_text(text));
        XAML_RETURN_IF_FAILED(btn->set_halignment(xaml_halignment_center));
        XAML_RETURN_IF_FAILED(btn->set_valignment(xaml_valignment_bottom));

        xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new(
            [diagram](xaml_object *, xaml_event_args *) noexcept -> xaml_result {
                XAML_RETURN_IF_FAILED(ResetTitle(diagram, "Curve #1"));
                XAML_RETURN_IF_FAILED(ResetTitle(diagram, "Curve #2"));
                return XAML_S_OK;
            },
            &callback)));
        int32_t token;
        XAML_RETURN_IF_FAILED(btn->add_click(callback, &token));
        XAML_RETURN_IF_FAILED(g2->add_child(btn));
        XAML_RETURN_IF_FAILED(xaml_grid_set_column(btn, 2));
        XAML_RETURN_IF_FAILED(xaml_grid_set_row(btn, 0));
    }
    {
        xaml_ptr<xaml_button> btn;
        XAML_RETURN_IF_FAILED(xaml_button_new(&btn));
        xaml_ptr<xaml_string> text;
        XAML_RETURN_IF_FAILED(xaml_string_new("Make step", &text));
        XAML_RETURN_IF_FAILED(btn->set_text(text));
        XAML_RETURN_IF_FAILED(btn->set_halignment(xaml_halignment_center));
        XAML_RETURN_IF_FAILED(btn->set_valignment(xaml_valignment_bottom));

        xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new(
            [diagram](xaml_object *, xaml_event_args *) noexcept -> xaml_result {
                XAML_RETURN_IF_FAILED(CreateCurveData(diagram, "Curve #1"));
                XAML_RETURN_IF_FAILED(CreateCurveData(diagram, "Curve #2"));
                return XAML_S_OK;
            },
            &callback)));
        int32_t token;
        XAML_RETURN_IF_FAILED(btn->add_click(callback, &token));
        XAML_RETURN_IF_FAILED(g2->add_child(btn));
        XAML_RETURN_IF_FAILED(xaml_grid_set_column(btn, 3));
        XAML_RETURN_IF_FAILED(xaml_grid_set_row(btn, 0));
    }
    {
        xaml_ptr<xaml_button> btn;
        XAML_RETURN_IF_FAILED(xaml_button_new(&btn));
        xaml_ptr<xaml_string> text;
        XAML_RETURN_IF_FAILED(xaml_string_new("Reset", &text));
        XAML_RETURN_IF_FAILED(btn->set_text(text));
        XAML_RETURN_IF_FAILED(btn->set_halignment(xaml_halignment_center));
        XAML_RETURN_IF_FAILED(btn->set_valignment(xaml_valignment_bottom));

        xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new(
            [diagram](xaml_object *, xaml_event_args *) noexcept -> xaml_result {
                return diagram->set_reset(true);
            },
            &callback)));
        int32_t token;
        XAML_RETURN_IF_FAILED(btn->add_click(callback, &token));
        XAML_RETURN_IF_FAILED(g2->add_child(btn));
        XAML_RETURN_IF_FAILED(xaml_grid_set_column(btn, 4));
        XAML_RETURN_IF_FAILED(xaml_grid_set_row(btn, 0));
    }
    XAML_RETURN_IF_FAILED(g->add_child(g2));
    XAML_RETURN_IF_FAILED(xaml_grid_set_column(g2, 0));
    XAML_RETURN_IF_FAILED(xaml_grid_set_row(g2, 1));

    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_test_window_new(xaml_test_window ** ptr) noexcept
{
    return xaml_object_init<xaml_test_window_impl>(ptr);
}
