#pragma once

#include <unordered_map>

#include <QwtPlot>

class QwtLegend;
class QwtPlotCurve;

class DiagramControl : public QwtPlot
{
    Q_OBJECT
public:
    DiagramControl(QWidget * parent = nullptr);

    void addCurve(const QString & id, const QString & title, const QString & color);

    void addSeries(const QString & curveSeriesData);

    void resetCurves();

    void setAxisScale(QwtAxisId axisId, const QString & scaleData);

    void setCurveColor(const QString & curveColorData);

    void setCurveTitle(const QString & curveTitleData);

private:
    class DiagramData;
    std::unordered_map<QString, QwtPlotCurve *> m_curves;

    QwtPlotCurve * detectCurve(const QStringList & data) const;

    DiagramData * detectDiagramData(const QStringList & data) const;
};
