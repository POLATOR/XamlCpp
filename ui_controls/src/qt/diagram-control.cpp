#include <qt/diagram-control.h>
#include <xaml/ui/controls/diagram.h>

#include <QPen>

#include <QwtPlotCurve>
#include <QwtPlotGrid>
#include <QwtPlotPanner>
#include <QwtPlotOpenGLCanvas>
#include <QwtPlotLegendItem>

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

    auto plotCanvas = new QwtPlotOpenGLCanvas();
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

void DiagramControl::addSeries(const QString & curveDataData)
{
    auto data = parseData(curveDataData);
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
    auto data = parseData(scaleData);
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
    auto data = parseData(curveColorData);
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
    auto data = parseData(curveTitleData);
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

QStringList DiagramControl::parseData(const QString & data)
{
    // TODO make more smart function?
    return data.split(DIAGRAM_COMMAND_SEPARATOR, Qt::SkipEmptyParts);
}

QwtPlotCurve * DiagramControl::detectCurve(const QStringList & data)
{
    if (data.isEmpty()) {
        return nullptr;
    }
    auto found = m_curves.find(data.front());
    if (found == m_curves.end()) {
        auto curve = new QwtPlotCurve();
        curve->setTitle(data.front());
        curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
        curve->setData(new DiagramData);
        curve->attach(this);
        auto rv = m_curves.emplace(data.front(), curve);
        if (!rv.second) {
            delete curve;
            return nullptr;
        }
        found = rv.first;
    }
    return found->second;
}

DiagramControl::DiagramData * DiagramControl::detectDiagramData(const QStringList & data)
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
