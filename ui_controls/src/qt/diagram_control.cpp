#include <qt/diagram_control.h>
#include <xaml/ui/controls/diagram.h>

#include <QPen>
#include <QDebug>

#include <QwtPlotCurve>
#include <QwtPlotGrid>
#include <QwtPlotPanner>
#include <QwtPlotCanvas>
#include <QwtPlotLegendItem>
#include <QwtText>

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
    m_grid = new QwtPlotGrid;
    m_grid->attach(this);

    auto plotCanvas = new QwtPlotCanvas();
    plotCanvas->setFrameStyle(QFrame::Box | QFrame::Plain);
    plotCanvas->setLineWidth(1);
    setCanvas(plotCanvas);

    auto panner = new QwtPlotPanner(plotCanvas);
    panner->setAxisEnabled(QwtAxis::XBottom, true);

    m_legend = new QwtPlotLegendItem();
    m_legend->setMaxColumns(1);
    m_legend->setRenderHint(QwtPlotItem::RenderAntialiased);
    m_legend->setAlignmentInCanvas(Qt::AlignRight | Qt::AlignTop);
    m_legend->setItemMargin(0);
    m_legend->attach(this);

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

QColor DiagramControl::gridColor() const
{
    return m_grid->majorPen().color();
}

void DiagramControl::setGridColor(const QColor & gridColor)
{
    m_grid->setPen(gridColor);
}

QColor DiagramControl::legendBackgroundColor() const
{
    return m_legend->backgroundBrush().color();
}
void DiagramControl::setLegendBackgroundColor(const QColor & legendBackgroundColor)
{
    m_legend->setBackgroundBrush(legendBackgroundColor);
}

QColor DiagramControl::legendBorderColor() const
{
    return m_legend->borderPen().color();
}
void DiagramControl::setLegendBorderColor(const QColor & legendBorderColor)
{
    m_legend->setBorderPen(legendBorderColor);
}

QColor DiagramControl::legendTextColor() const
{
    return m_legend->textPen().color();
}
void DiagramControl::setLegendTextColor(const QColor & legendTextColor)
{
    m_legend->setTextPen(legendTextColor);
}

int DiagramControl::legendMargin() const
{
    return m_legend->margin();
}
void DiagramControl::setLegendMargin(int legendMargin)
{
    m_legend->setMargin(legendMargin);
}

int DiagramControl::legendSpacing() const
{
    return m_legend->spacing();
}
void DiagramControl::setLegendSpacing(int legendSpacing)
{
    m_legend->setSpacing(legendSpacing);
}

double DiagramControl::legendBorderRadius() const
{
    return m_legend->borderRadius();
}
void DiagramControl::setLegendBorderRadius(double legendBorderRadius)
{
    m_legend->setBorderRadius(legendBorderRadius);
}

int DiagramControl::scaleTitleFontPixelSize() const
{
    return axisTitle(QwtAxis::YLeft).font().pointSize();
}

void DiagramControl::setScaleTitleFontPixelSize(int scaleTitleFontPixelSize)
{
    updateAxisTitleFontPixelSize(QwtAxis::XBottom, scaleTitleFontPixelSize);
    updateAxisTitleFontPixelSize(QwtAxis::XTop, scaleTitleFontPixelSize);
    updateAxisTitleFontPixelSize(QwtAxis::YLeft, scaleTitleFontPixelSize);
    updateAxisTitleFontPixelSize(QwtAxis::YRight, scaleTitleFontPixelSize);
}

int DiagramControl::scaleTitleFontWeight() const
{
    return axisTitle(QwtAxis::YLeft).font().weight();
}
void DiagramControl::setScaleTitleFontWeight(int scaleTitleFontWeight)
{
    updateAxisTitleFontWeight(QwtAxis::XBottom, scaleTitleFontWeight);
    updateAxisTitleFontWeight(QwtAxis::XTop, scaleTitleFontWeight);
    updateAxisTitleFontWeight(QwtAxis::YLeft, scaleTitleFontWeight);
    updateAxisTitleFontWeight(QwtAxis::YRight, scaleTitleFontWeight);
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

void DiagramControl::updateAxisTitleFontPixelSize(QwtAxisId id, int fontSize)
{
    auto title = axisTitle(id);
    auto font = title.font();
    font.setPixelSize(fontSize);
    title.setFont(font);
    setAxisTitle(id, title);
}

void DiagramControl::updateAxisTitleFontWeight(QwtAxisId id, int fontWeight)
{
    auto title = axisTitle(id);
    auto font = title.font();
    font.setWeight(static_cast<QFont::Weight>(fontWeight));
    title.setFont(font);
    setAxisTitle(id, title);
}
