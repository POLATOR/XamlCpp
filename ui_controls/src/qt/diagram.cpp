#include <qt/qstring.hpp>
#include <qt/diagram_control.h>
#include <shared/diagram.hpp>

xaml_result xaml_diagram_internal::draw(xaml_rectangle const & region) noexcept
{
    if (!m_handle) {
        XAML_RETURN_IF_FAILED(create<DiagramControl>());
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(setXLabel());
        XAML_RETURN_IF_FAILED(setXRange());
        XAML_RETURN_IF_FAILED(setYLabel());
        XAML_RETURN_IF_FAILED(setYRange());
        XAML_RETURN_IF_FAILED(addCurveData());
    }
    return set_rect(region);
}

xaml_result xaml_diagram_internal::setXLabel() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString label;
        XAML_RETURN_IF_FAILED(to_QString(m_xScaleLabel, &label));
        plot->setAxisTitle(QwtAxis::XBottom, label);
    }
    return XAML_S_OK;
}

xaml_result xaml_diagram_internal::setXRange() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString scaleData;
        XAML_RETURN_IF_FAILED(to_QString(m_xScaleRange, &scaleData));
        plot->setAxisScale(QwtAxis::XBottom, scaleData);
    }
    return XAML_S_OK;
}

xaml_result xaml_diagram_internal::setYLabel() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString label;
        XAML_RETURN_IF_FAILED(to_QString(m_yScaleLabel, &label));
        plot->setAxisTitle(QwtAxis::YLeft, label);
    }
    return XAML_S_OK;
}

xaml_result xaml_diagram_internal::setYRange() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString scaleData;
        XAML_RETURN_IF_FAILED(to_QString(m_yScaleRange, &scaleData));
        plot->setAxisScale(QwtAxis::YLeft, scaleData);
    }
    return XAML_S_OK;
}

xaml_result xaml_diagram_internal::setCurveColor() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString colorData;
        XAML_RETURN_IF_FAILED(to_QString(m_curveColor, &colorData));
        plot->setCurveColor(colorData);
    }
    return XAML_S_OK;
}

xaml_result xaml_diagram_internal::setCurveTitle() noexcept
{
    if (auto plot = qobject_cast<DiagramControl *>(m_handle)) {
        QString titleData;
        XAML_RETURN_IF_FAILED(to_QString(m_curveTitle, &titleData));
        plot->setCurveTitle(titleData);
    }
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_diagram_internal::addCurveData() noexcept
{
    if (auto plot = dynamic_cast<DiagramControl *>(m_handle)) {
        QString curveDataData;
        XAML_RETURN_IF_FAILED(to_QString(m_curveData, &curveDataData));
        plot->addSeries(curveDataData);
    }
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_diagram_internal::reset() noexcept
{
    if (auto plot = dynamic_cast<DiagramControl *>(m_handle)) {
        plot->resetCurves();
    }
    return XAML_S_OK;
}
