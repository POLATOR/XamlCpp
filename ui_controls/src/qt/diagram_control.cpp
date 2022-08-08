#include <qt/diagram_control.h>
#include <xaml/ui/controls/diagram.h>

#include <QPen>

#include <QwtPlotCurve>
#include <QwtPlotGrid>
#include <QwtPlotPanner>
#include <QwtPlotCanvas>
#include <QwtPlotLegendItem>

namespace {

QStringList ParsePair(const QString & string)
{
    auto pos = string.indexOf(DIAGRAM_COMMAND_SEPARATOR);
    if (pos == -1 || pos == 0) {
        return {};
    }
    return {string.mid(0, pos), string.mid(pos + 1)};
}

} // namespace

class DiagramControl::DiagramData : public QwtSeriesData<QPointF>
{
public:
    using QwtSeriesData<QPointF>::QwtSeriesData;

    void clear()
    {
        m_data.clear();
    }

    size_t size() const override
    {
        return m_data.size();
    }

    QPointF sample(size_t index) const override
    {
        return m_data[index];
    }

    QRectF boundingRect() const override
    {
        return cachedBoundingRect;
    }

    void addPoints(const QStringList & data)
    {
        for (qsizetype i = 1; i < data.count() - 1; i += 2) {
            bool ok = false;
            auto x = data[i].toDouble(&ok);
            if (ok) {
                ok = false;
                auto y = data[i + 1].toDouble(&ok);
                if (ok) {
                    m_data.emplaceBack(x, y);
                    updateBound(x, y);
                }
            }
        }
    }

private:
    QList<QPointF> m_data;

    void updateBound(double x, double y)
    {
        if (x < cachedBoundingRect.left()) {
            cachedBoundingRect.setLeft(x);
        }
        if (x > cachedBoundingRect.right()) {
            cachedBoundingRect.setRight(x);
        }
        if (y < cachedBoundingRect.top()) {
            cachedBoundingRect.setTop(y);
        }
        if (y > cachedBoundingRect.bottom()) {
            cachedBoundingRect.setBottom(y);
        }
    }
};

DiagramControl::DiagramControl(QWidget * parent)
    : QwtPlot(parent)
{
    auto grid = new QwtPlotGrid;
    grid->setPen(QColor("#8C8C8C"));
    grid->attach(this);

    auto plotCanvas = new QwtPlotCanvas();
    plotCanvas->setPalette(QColor(QColorConstants::Svg::white));
    plotCanvas->setFrameStyle(QFrame::Box | QFrame::Plain);
    plotCanvas->setLineWidth(1);
    setCanvas(plotCanvas);

    auto panner = new QwtPlotPanner(plotCanvas);
    panner->setAxisEnabled(QwtAxis::XBottom, true);

    auto legendItem = new QwtPlotLegendItem();
    legendItem->setMaxColumns(1);
    legendItem->setRenderHint(QwtPlotItem::RenderAntialiased);
    legendItem->setBackgroundBrush(Qt::white);
    legendItem->setBorderPen(QColor("#8C8C8C"));
    legendItem->setTextPen(QColor("#8C8C8C"));
    legendItem->setAlignmentInCanvas(Qt::AlignRight | Qt::AlignTop);
    legendItem->setBorderRadius(4);
    legendItem->setMargin(20);
    legendItem->setSpacing(10);
    legendItem->setItemMargin(0);
    legendItem->attach(this);

    replot();
}

void DiagramControl::addCurve(const QString & id, const QString & title, const QString & color)
{
    auto found = m_curves.find(id);
    if (found != m_curves.end()) {
        return;
    }
    auto curve = new QwtPlotCurve();
    curve->setTitle(title);
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve->setData(new DiagramData);
    curve->setPen(QColor(color));
    curve->attach(this);
    auto rv = m_curves.emplace(id, curve);
    if (!rv.second) {
        delete curve;
    }
    // QMetaObject::invokeMethod(canvas(), "replot", Qt::DirectConnection);
}

void DiagramControl::addSeries(const QString & curveSeriesData)
{
    auto data = curveSeriesData.split(DIAGRAM_COMMAND_SEPARATOR, Qt::SkipEmptyParts);
    auto diagramData = detectDiagramData(data);
    if (!diagramData) {
        return;
    }
    diagramData->addPoints(data);
    QMetaObject::invokeMethod(canvas(), "replot", Qt::DirectConnection);
}

void DiagramControl::resetCurves()
{
    for (auto & curve: m_curves) {
        delete curve.second;
    }
    m_curves.clear();
    QMetaObject::invokeMethod(canvas(), "replot", Qt::DirectConnection);
}

void DiagramControl::setAxisScale(QwtAxisId axisId, const QString & scaleData)
{
    auto data = ParsePair(scaleData);
    if (data.size() != 2) {
        return;
    }
    bool ok = false;
    auto min = data[0].toDouble(&ok);
    if (ok) {
        ok = false;
        auto max = data[1].toDouble(&ok);
        if (ok) {
            QwtPlot::setAxisScale(axisId, min, max);
            replot();
        }
    }
}

void DiagramControl::setCurveColor(const QString & curveColorData)
{
    auto data = ParsePair(curveColorData);
    if (data.size() != 2) {
        return;
    }
    auto curve = detectCurve(data);
    if (!curve) {
        return;
    }
    curve->setPen(QColor(data[1]));
    QMetaObject::invokeMethod(canvas(), "replot", Qt::DirectConnection);
}

void DiagramControl::setCurveTitle(const QString & curveTitleData)
{
    auto data = ParsePair(curveTitleData);
    if (data.size() != 2) {
        return;
    }
    auto curve = detectCurve(data);
    if (!curve) {
        return;
    }
    curve->setTitle(data[1]);
    QMetaObject::invokeMethod(canvas(), "replot", Qt::DirectConnection);
}

QwtPlotCurve * DiagramControl::detectCurve(const QStringList & data) const
{
    if (data.isEmpty()) {
        return nullptr;
    }
    auto found = m_curves.find(data.front());
    if (found == m_curves.end()) {
        return nullptr;
    }
    return found->second;
}

DiagramControl::DiagramData * DiagramControl::detectDiagramData(const QStringList & data) const
{
    if (data.size() < 3) {
        return nullptr;
    }
    auto curve = detectCurve(data);
    if (!curve) {
        return nullptr;
    }
    return static_cast<DiagramData *>(curve->data());
}
