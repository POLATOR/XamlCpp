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

    void addSeries(const QString & curveDataData);

    void resetCurves();

    void setAxisScale(QwtAxisId axisId, const QString & scaleData);

    void setCurveColor(const QString & curveColorData);

    void setCurveTitle(const QString & curveTitleData);

private:
    class DiagramData;
    std::unordered_map<QString, QwtPlotCurve *> m_curves;

    static QStringList parseData(const QString & data);

    QwtPlotCurve * detectCurve(const QStringList & data);

    DiagramData * detectDiagramData(const QStringList & data);
};
