#ifndef LINECHARTVIEW_H
#define LINECHARTVIEW_H

#include <QWidget>
#include <QtCharts>
#include <QChart>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include "hermite.h"
#include "spline.h"

namespace Ui {
class LineChartView;
}

class CustomChartView : public QChartView
{
    Q_OBJECT
public:
    CustomChartView(QChart *chartInput, QWidget *parent = nullptr);

    struct Point{
        QPointF pt;
        QVector2D tangent;
        float weight = 1.0f;
        Point(){}
        Point(const QPointF& pt, float w):pt(pt), weight(w){}
    };

    QList<Point> m_controlPoints;    // 用户绘制的控制点。
    QList<QPointF> m_splinePoints;  // 插值后的样条曲线点。
    int curCtrlPointID=-1;   // 当前选择的控制点ID。

    void addPoint(Point pt);    // 新增一个用户绘制的控制点。
    void updatePoints();          // 更新表格（当控制点变化时调用）。
    static QVector<QPointF> GetSmoothedLine(const QVector<QPointF> &controlPoints);   // 根据控制点，返回光滑的插值样条曲线点。

protected:
    void mousePressEvent(QMouseEvent *event) override ;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    QLineSeries *m_lineSeries;     // 光滑样条曲线点对应的Series。
    QScatterSeries *m_pointSeries;  // 用户绘制的控制点对应的Series。
    QScatterSeries *m_selectPointSeries;  // 用户选中某个顶点时，那个单独的顶点对应的Series。



};



class LineChartView : public QDialog
{
    Q_OBJECT

public:
    explicit LineChartView(QWidget *parent = 0);
    ~LineChartView();

    void SetControlPoints(const QVector<QPointF>& ctrlPoints);
    QVector<QPointF> GetControlPoints();
    QVector<QPointF> GetSplinePoints();

private:
    QChart *chart = nullptr;
    CustomChartView *chartView = nullptr;
};

#endif // LINECHARTVIEW_H
