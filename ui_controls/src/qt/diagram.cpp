#include <qt/qstring.hpp>
#include <qt/diagram_control.h>
#include <shared/diagram.hpp>

xaml_result xaml_diagram_internal::draw(xaml_rectangle const & region) noexcept
{
    if (!m_handle) {
        XAML_RETURN_IF_FAILED(create<DiagramControl>());
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(init_curves());
        XAML_RETURN_IF_FAILED(setXLabel());
        XAML_RETURN_IF_FAILED(setXRange());
        XAML_RETURN_IF_FAILED(setYLabel());
        XAML_RETURN_IF_FAILED(setYRange());
        XAML_RETURN_IF_FAILED(addCurveData());
    }
    return set_rect(region);
}

xaml_result XAML_CALL xaml_diagram_internal::init_curves() noexcept
{
    if (auto diagram = qobject_cast<DiagramControl *>(m_handle)) {
        diagram->resetCurves();
        if (m_curves) {
            using kvp = xaml_key_value_pair<xaml_string, xaml_diagram_curve>;
            XAML_FOREACH_START(kvp, it, m_curves);
            {
                xaml_ptr<xaml_diagram_curve> value;
                XAML_RETURN_IF_FAILED(it->get_value(&value));
                if (value) {
                    QString id, title, color;
                    {
                        xaml_ptr<xaml_string> str;
                        XAML_RETURN_IF_FAILED(value->get_id(&str));
                        XAML_RETURN_IF_FAILED(to_QString(str, &id));
                    }
                    {
                        xaml_ptr<xaml_string> str;
                        XAML_RETURN_IF_FAILED(value->get_title(&str));
                        XAML_RETURN_IF_FAILED(to_QString(str, &title));
                    }
                    {
                        xaml_ptr<xaml_string> str;
                        XAML_RETURN_IF_FAILED(value->get_color(&str));
                        XAML_RETURN_IF_FAILED(to_QString(str, &color));
                    }
                    diagram->addCurve(id, title, color);
                }
            }
            XAML_FOREACH_END();
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_diagram_internal::setXLabel() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString label;
        XAML_RETURN_IF_FAILED(to_QString(m_x_scale_label, &label));
        plot->setAxisTitle(QwtAxis::XBottom, label);
    }
    return XAML_S_OK;
}

xaml_result xaml_diagram_internal::setXRange() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString scaleData;
        XAML_RETURN_IF_FAILED(to_QString(m_x_scale_range, &scaleData));
        plot->setAxisScale(QwtAxis::XBottom, scaleData);
    }
    return XAML_S_OK;
}

xaml_result xaml_diagram_internal::setYLabel() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString label;
        XAML_RETURN_IF_FAILED(to_QString(m_y_scale_label, &label));
        plot->setAxisTitle(QwtAxis::YLeft, label);
    }
    return XAML_S_OK;
}

xaml_result xaml_diagram_internal::setYRange() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString scaleData;
        XAML_RETURN_IF_FAILED(to_QString(m_y_scale_range, &scaleData));
        plot->setAxisScale(QwtAxis::YLeft, scaleData);
    }
    return XAML_S_OK;
}

xaml_result xaml_diagram_internal::setCurveColor() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString colorData;
        XAML_RETURN_IF_FAILED(to_QString(m_curve_color, &colorData));
        plot->setCurveColor(colorData);
    }
    return XAML_S_OK;
}

xaml_result xaml_diagram_internal::setCurveTitle() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString titleData;
        XAML_RETURN_IF_FAILED(to_QString(m_curve_title, &titleData));
        plot->setCurveTitle(titleData);
    }
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_diagram_internal::addCurveData() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString curve_dataData;
        XAML_RETURN_IF_FAILED(to_QString(m_curve_data, &curve_dataData));
        plot->addSeries(curve_dataData);
    }
    return XAML_S_OK;
}
