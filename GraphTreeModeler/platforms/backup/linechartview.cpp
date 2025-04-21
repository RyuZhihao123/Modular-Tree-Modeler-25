#include "linechartview.h"
#include "ui_linechartview.h"

LineChartView::LineChartView(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Spline Controller");

    chart = new QChart();

    // 自定义图表视图
    chartView = new CustomChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->show();
}


void LineChartView::SetControlPoints(const QVector<QPointF> &ctrlPoints)
{
    chartView->m_controlPoints.clear();
    chartView->m_splinePoints.clear();
    chartView->curCtrlPointID = -1;

    for(int i=0; i<ctrlPoints.size(); ++i)
        chartView->m_controlPoints.append(CustomChartView::Point(ctrlPoints[i],1.0f));

    chartView->updatePoints();
}

QVector<QPointF> LineChartView::GetControlPoints()
{
    QVector<QPointF> controlPoints;
    for(int i=0; i<chartView->m_controlPoints.size(); ++i)
        controlPoints.append(chartView->m_controlPoints[i].pt);
    return controlPoints;
}

QVector<QPointF> LineChartView::GetSplinePoints()
{
    QVector<QPointF> splinePoints;
    for(int i=0; i<chartView->m_splinePoints.size(); ++i)
        splinePoints.append(chartView->m_splinePoints[i]);
    return splinePoints;
}


CustomChartView::CustomChartView(QChart *chartInput, QWidget *parent)
    : QChartView(chartInput, parent)
{
    // 初始化
    this->chart()->setTitle("Spline Controller");
    this->chart()->setTheme(QChart::ChartThemeBlueCerulean);

    this->m_lineSeries = new QLineSeries();
    this->chart()->addSeries(m_lineSeries);
    this->m_pointSeries = new QScatterSeries();
    this->chart()->addSeries(m_pointSeries);
    this->m_selectPointSeries = new QScatterSeries();
    this->chart()->addSeries(m_selectPointSeries);
    m_selectPointSeries->setColor(Qt::red);

    this->chart()->legend()->setVisible(false);

    this->chart()->createDefaultAxes();
    this->chart()->axisX()->setRange(0, 1);
    this->chart()->axisY()->setRange(-0.2, 1.2);
    this->chart()->axisY()->setVisible(false);
    this->resize(500, 250);

//   this->updatePoints();
}


void CustomChartView::addPoint(Point pt)
{
    if(pt.pt.y()>1.0)
        pt.pt.setY(1.0f);
    for(int i=0; i<m_controlPoints.count()-1;++i)
    {
        auto prev = m_controlPoints[i+0].pt;
        auto next = m_controlPoints[i+1].pt;
        if(pt.pt.x() > prev.x() && pt.pt.x() < next.x())
            m_controlPoints.insert(i+1, pt);
    }
    this->updatePoints();
}

void CustomChartView::updatePoints()
{
    this->m_pointSeries->clear();
    this->m_lineSeries->clear();
    this->m_splinePoints.clear();

    if(m_controlPoints.size() < 2)
        return;

    // 首先将控制点加入到ScatterSeries中。
    QVector<QPointF> _originalPts;
    for(int i=0; i<m_controlPoints.size(); ++i)
    {
        m_pointSeries->append(m_controlPoints[i].pt);
        _originalPts.append(m_controlPoints[i].pt);
    }

    // 将光滑的样条曲线加入到LineSeries中。
    this->m_splinePoints = this->GetSmoothedLine(_originalPts).toList();  // 生成插值后的光滑样条点。

    for(int i=0; i<m_splinePoints.size(); ++i)
        this->m_lineSeries->append(m_splinePoints[i]);
}


QVector<QPointF> CustomChartView::GetSmoothedLine(const QVector<QPointF> &controlPoints)
{
    using namespace SplineSpace;

    QVector<QPointF> sampled_points;
    for(int i=0; i<controlPoints.size(); ++i)
        sampled_points.append(controlPoints[i]);

    if(sampled_points.size() == 2)
    {
        sampled_points.insert(1, (sampled_points[0] + sampled_points[1])/2.0f);
    }

    int size = sampled_points.size();
    double x[size];   // 0-(size-1)
    double _x[size]; double _y[size]; double _z[size];

    // 加入数据
    for(unsigned int k=0; k<sampled_points.size(); ++k)
    {
        x[k] = k;
        _x[k] = sampled_points[k].x();
        _y[k] = sampled_points[k].y();
        _z[k] = 0.0f;
    }

    Spline cs_x(x,_x,size);
    Spline cs_y(x,_y,size);
    Spline cs_z(x,_z,size);

    int t = 7; // 中间新增的个数 t必须为奇数
    double  newX[size+(size-1)*t];   // 0 _ _ _ 1 _ _ _  2 _ _ _ 3 [size = 4] -> size=(size+(size-1)*t)
    double new_X[size+(size-1)*t];
    double new_Y[size+(size-1)*t];
    double new_Z[size+(size-1)*t];
    for (float i = 0.0f; i <= (size-1); i += 1.0f/(t+1))
        newX[(int)(i*(t+1))] = i;

    cs_x.MultiPointInterp(newX, size+(size-1)*t,new_X);
    cs_y.MultiPointInterp(newX,size+(size-1)*t,new_Y);
    cs_z.MultiPointInterp(newX, size+(size-1)*t,new_Z);

    QVector<QPointF> result;
    for (int i=0; i<size+(size-1)*t; ++i)
    {
        result.push_back(QPointF(new_X[i], new_Y[i]));
    }

    return result;
}


void CustomChartView::mousePressEvent(QMouseEvent *event)
{
    QChartView::mousePressEvent(event); // 保留默认行为

    if (event->buttons() & Qt::RightButton) {
        // 将鼠标视图坐标转换为图表的值坐标
        Point ctrlPt(chart()->mapToValue(event->pos(), m_lineSeries), 1.0f);
        this->addPoint(ctrlPt);
    }
    if(event->buttons()& Qt::LeftButton)
    {
        Point pressPt(chart()->mapToValue(event->pos(), m_lineSeries), 1.0f);
        float dist = 999;
        this->curCtrlPointID = -1;
        m_selectPointSeries->clear();
        for(int i=0; i<this->m_controlPoints.size(); ++i)
        {
            auto pt = m_controlPoints[i].pt;
            float curDist = (pressPt.pt - pt).manhattanLength();
            if(curDist <dist && curDist < 0.25f)
            {
                dist = curDist;
                curCtrlPointID = i;
            }
        }

        if(curCtrlPointID !=-1)
        {
            m_selectPointSeries->append(this->m_controlPoints[curCtrlPointID].pt);
        }
    }
}

void CustomChartView::mouseMoveEvent(QMouseEvent *event)
{
    QChartView::mouseMoveEvent(event); // 保留默认行为


    if(event->buttons()& Qt::LeftButton && curCtrlPointID != -1)
    {
        m_selectPointSeries->clear();
        if(curCtrlPointID == 0 || curCtrlPointID == (m_controlPoints.size() -1))
        {
            QPointF newPos = chart()->mapToValue(event->pos(), m_lineSeries);
            newPos.setY(min(newPos.y(), qreal(1.0f)));
            newPos.setY(max(newPos.y(), qreal(0.0f)));

            m_controlPoints[curCtrlPointID].pt.setY(newPos.y());
            if(curCtrlPointID !=-1)
                m_selectPointSeries->append(m_controlPoints[curCtrlPointID].pt);
        }
        else
        {
            m_selectPointSeries->clear();
            QPointF prev = m_controlPoints[curCtrlPointID-1].pt;
            QPointF next = m_controlPoints[curCtrlPointID+1].pt;

            QPointF newPos = chart()->mapToValue(event->pos(), m_lineSeries);

            newPos.setX(max(newPos.x(), prev.x() + 0.1f)); // 范围限定；
            newPos.setX(min(newPos.x(), next.x() - 0.1f));
            newPos.setY(min(newPos.y(), qreal(1.0f)));
            newPos.setY(max(newPos.y(), qreal(0.0f)));

            m_controlPoints[curCtrlPointID].pt= newPos;
            if(curCtrlPointID !=-1)
                m_selectPointSeries->append(newPos);
        }
        this->updatePoints();
        this->update();
    }
}

void CustomChartView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
    {
        if(curCtrlPointID != -1 && curCtrlPointID != 0 && curCtrlPointID != (m_controlPoints.size()-1))
        {
            this->m_controlPoints.removeAt(curCtrlPointID);
            curCtrlPointID = -1;
            m_selectPointSeries->clear();
            this->updatePoints();
        }
    }
}

LineChartView::~LineChartView()
{
    delete this->chart;
    delete this->chartView;
//    delete ui;
}
