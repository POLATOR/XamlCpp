#pragma once

#include <unordered_map>

#include <QwtPlot>

class QwtPlotCurve;
class QwtPlotGrid;
class QwtPlotLegendItem;

class DiagramControl : public QwtPlot
{
    Q_OBJECT
    Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor)
    Q_PROPERTY(QColor legendBackgroundColor READ legendBackgroundColor WRITE setLegendBackgroundColor)
    Q_PROPERTY(QColor legendBorderColor READ legendBorderColor WRITE setLegendBorderColor)
    Q_PROPERTY(QColor legendTextColor READ legendTextColor WRITE setLegendTextColor)
    Q_PROPERTY(double legendBorderRadius READ legendBorderRadius WRITE setLegendBorderRadius)
    Q_PROPERTY(int legendMargin READ legendMargin WRITE setLegendMargin)
    Q_PROPERTY(int legendSpacing READ legendSpacing WRITE setLegendSpacing)
    Q_PROPERTY(int scaleTitleFontPixelSize READ scaleTitleFontPixelSize WRITE setScaleTitleFontPixelSize)
    Q_PROPERTY(int scaleTitleFontWeight READ scaleTitleFontWeight WRITE setScaleTitleFontWeight)
public:
    DiagramControl(QWidget * parent = nullptr);

    void addCurve(const QString & id, const QString & title, const QString & color);

    void addSeries(const QString & curveSeriesData);

    void resetCurves();

    void setAxisScale(QwtAxisId axisId, const QString & scaleData);

    void setCurveColor(const QString & curveColorData);

    void setCurveTitle(const QString & curveTitleData);

    QColor gridColor() const;
    void setGridColor(const QColor & gridColor);

    QColor legendBackgroundColor() const;
    void setLegendBackgroundColor(const QColor & legendBackgroundColor);

    QColor legendBorderColor() const;
    void setLegendBorderColor(const QColor & legendBorderColor);

    QColor legendTextColor() const;
    void setLegendTextColor(const QColor & legendTextColor);

    int legendMargin() const;
    void setLegendMargin(int legendMargin);

    int legendSpacing() const;
    void setLegendSpacing(int legendSpacing);

    double legendBorderRadius() const;
    void setLegendBorderRadius(double legendBorderRadius);

    int scaleTitleFontPixelSize() const;
    void setScaleTitleFontPixelSize(int scaleTitleFontPixelSize);

    int scaleTitleFontWeight() const;
    void setScaleTitleFontWeight(int scaleTitleFontWeight);

private:
    class DiagramData;
    std::unordered_map<QString, QwtPlotCurve *> m_curves;
    QwtPlotGrid * m_grid;
    QwtPlotLegendItem * m_legend = nullptr;

    QwtPlotCurve * detectCurve(const QStringList & data) const;

    DiagramData * detectDiagramData(const QStringList & data) const;

    void updateAxisTitleFontPixelSize(QwtAxisId id, int fontSize);

    void updateAxisTitleFontWeight(QwtAxisId id, int fontWeight);
};
