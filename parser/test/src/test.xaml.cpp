#include <cmath>
#include <shared/window.hpp>
#include <test.xaml.h>
#include <test_converter.h>
#include <xaml/parser/deserializer.h>
#include <xaml/parser/parser.h>
#include <xaml/resource/resource.h>
#include <xaml/ui/application.h>
#include <xaml/ui/msgbox.h>
#include <xaml/ui/controls/diagram.h>

#include <random>
#include <sstream>

namespace colors {
#include <xaml/ui/colors.h>
}

namespace {

xaml_result CreateCurve(xaml_test_model * model, const std::string & curveName)
{
    std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<> dist(0.0, 1000.0);

    std::stringstream stream;
    stream << curveName;
    for (int i = 0; i < 100; ++i) {
        stream << DIAGRAM_COMMAND_SEPARATOR << dist(generator);
    }
    xaml_ptr<xaml_string> curve_data;
    XAML_RETURN_IF_FAILED(xaml_string_new(stream.str(), &curve_data));
    return model->set_curve_data(curve_data);
}

} // namespace

using namespace std;

struct xaml_test_window_internal : xaml_window_internal
{
    xaml_ptr<xaml_meta_context> m_ctx;

    xaml_result XAML_CALL init() noexcept;

    XAML_PROP_PTR_IMPL(model, xaml_test_model)

    xaml_result XAML_CALL on_button_click(xaml_button *) noexcept;
    xaml_result XAML_CALL on_canvas_redraw(xaml_canvas *, xaml_drawing_context *) noexcept;
};

struct xaml_test_window_impl : xaml_window_implement<xaml_test_window_impl, xaml_test_window_internal, xaml_test_window>
{
    xaml_result XAML_CALL init(xaml_meta_context * ctx) noexcept
    {
        m_internal.m_ctx = ctx;
        return m_internal.init();
    }

    XAML_PROP_PTR_INTERNAL_IMPL(model, xaml_test_model)

    xaml_result XAML_CALL on_button_click(xaml_object * sender, xaml_event_args *) noexcept override
    {
        xaml_ptr<xaml_button> btn;
        XAML_RETURN_IF_FAILED(sender->query(&btn));
        return m_internal.on_button_click(btn);
    }
    xaml_result XAML_CALL on_canvas_redraw(xaml_object * sender, xaml_drawing_context * dc) noexcept override
    {
        xaml_ptr<xaml_canvas> cv;
        XAML_RETURN_IF_FAILED(sender->query(&cv));
        return m_internal.on_canvas_redraw(cv, dc);
    }
};

xaml_result xaml_test_window_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_window_internal::init());
    xaml_ptr<xaml_string> path;
    XAML_RETURN_IF_FAILED(xaml_string_new_view(U("view/test.xaml"), &path));
    void const * data;
    int32_t size;
    XAML_RETURN_IF_FAILED(xaml_resource_get(path, &data, &size));
    xaml_ptr<xaml_string> data_str;
    XAML_RETURN_IF_FAILED(xaml_string_new_view_length((char const *)data, size - 1, &data_str));
    xaml_ptr<xaml_node> node;
    xaml_ptr<xaml_vector_view<xaml_string>> headers;
    XAML_RETURN_IF_FAILED(xaml_parser_parse_string(m_ctx, data_str, &node, &headers));
    XAML_RETURN_IF_FAILED(xaml_parser_deserialize_inplace(m_ctx, node, m_outer_this));
    xaml_ptr<xaml_observable_vector<xaml_object>> items;
    XAML_RETURN_IF_FAILED(m_model->get_items(&items));
    {
        xaml_ptr<xaml_string> item;
        XAML_RETURN_IF_FAILED(xaml_string_new("\u05e9\u05dc\u05d5\u05dd", &item));
        XAML_RETURN_IF_FAILED(items->append(item));
    }
    {
        xaml_ptr<xaml_string> item;
        XAML_RETURN_IF_FAILED(xaml_string_new("锟斤拷", &item));
        XAML_RETURN_IF_FAILED(items->append(item));
    }
    {
        xaml_ptr<xaml_string> item;
        XAML_RETURN_IF_FAILED(xaml_string_new("\u05e9-\u043c-\u03bd", &item));
        XAML_RETURN_IF_FAILED(items->append(item));
    }
    return XAML_S_OK;
}

xaml_result xaml_test_window_internal::on_button_click(xaml_button *) noexcept
{
    xaml_ptr<xaml_string> text;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello world!"), &text));
    XAML_RETURN_IF_FAILED(m_model->set_text(text));
    xaml_ptr<xaml_string> title;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("Hello"), &title));
    xaml_ptr<xaml_string> des;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("I'm going to say hello..."), &des));
    xaml_msgbox_result res;
    XAML_RETURN_IF_FAILED(
        xaml_msgbox(static_cast<xaml_window *>(m_outer_this), text, title, des, xaml_msgbox_info, xaml_msgbox_buttons_abort_retry_ignore, &res));
    switch (res) {
    case xaml_msgbox_result_abort:
        XAML_RETURN_IF_FAILED(xaml_string_new(U("Abort"), text.put()));
        break;
    case xaml_msgbox_result_retry:
        XAML_RETURN_IF_FAILED(xaml_string_new(U("Retry"), text.put()));
        break;
    case xaml_msgbox_result_ignore:
        XAML_RETURN_IF_FAILED(xaml_string_new(U("Ignore"), text.put()));
        break;
    default:
        return XAML_S_OK;
    }
    XAML_RETURN_IF_FAILED(m_model->set_text(text));
    XAML_RETURN_IF_FAILED(CreateCurve(m_model, "Curve #2"));
    return XAML_S_OK;
}

xaml_result xaml_test_window_internal::on_canvas_redraw(xaml_canvas * cv, xaml_drawing_context * dc) noexcept
{
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
    XAML_RETURN_IF_FAILED(xaml_brush_pen_new_solid(is_dark ? colors::white : colors::black, 2, &pen1));
    XAML_RETURN_IF_FAILED(dc->draw_ellipse(pen1, {cx - r, cy - r, r * 2, r * 2}));
    xaml_point pc{cx, cy};
    xaml_point p1 = xaml_point{0, -r} + pc;
    xaml_point p2 = xaml_point{r * sqrt(3) / 2, r / 2} + pc;
    xaml_point p3 = xaml_point{-r * sqrt(3) / 2, r / 2} + pc;
    xaml_ptr<xaml_brush_pen> pen2;
    XAML_RETURN_IF_FAILED(xaml_brush_pen_new_solid(colors::sky_blue, 1, &pen2));
    XAML_RETURN_IF_FAILED(dc->draw_line(pen2, p1, p2));
    XAML_RETURN_IF_FAILED(dc->draw_line(pen2, p2, p3));
    XAML_RETURN_IF_FAILED(dc->draw_line(pen2, p3, p1));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_test_window_new(xaml_meta_context * ctx, xaml_test_window ** ptr) noexcept
{
    return xaml_object_init<xaml_test_window_impl>(ptr, ctx);
}

xaml_result XAML_CALL xaml_test_window_register(xaml_meta_context * ctx) noexcept
{
    xaml_ptr<xaml_string> xml_ns;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("https://polator.ru/schemas/vpl-1/parser/test/"), &xml_ns));
    xaml_ptr<xaml_string> ns;
    XAML_RETURN_IF_FAILED(xaml_string_new(U("xaml_test"), &ns));
    XAML_RETURN_IF_FAILED(ctx->add_namespace(xml_ns, ns));

    XAML_TYPE_INFO_NEW(xaml_test_window, "test.xaml.h");
    XAML_RETURN_IF_FAILED(xaml_window_members(__info));
    XAML_TYPE_INFO_ADD_PROP(model, xaml_test_model);
    XAML_TYPE_INFO_ADD_METHOD(on_button_click, xaml_object, xaml_event_args);
    XAML_TYPE_INFO_ADD_METHOD(on_canvas_redraw, xaml_object, xaml_drawing_context);
    return ctx->add_type(__info);
}
