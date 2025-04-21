#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#include <QVector>  // std::vector<int...>
#include <QPointF>
#include <QDebug>
#include <QMap>  // std::map
#include <QVector3D>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QQueue>
#include "globalsetting.h"
#include "linechartview.h"

class BaseLevel
{
public:
    int seed = 1914;
    QString name = "name";          // 结点编号。
    QString type = "BaseLevel";     // 结点类型。
    QPoint pos;                               // Qt绘图时的坐标（不使用）。

    void UpdateNameUsingTime()   // 随机生成一个唯一的名字。
    {
        QDateTime time = QDateTime::currentDateTime();
        name = QString("%1").arg(time.toString("yyyy-MM-dd-hhmmsszzz"));
    }


    QString toString();
    static int paseBaseLevel(QStringList lines, int StartID, BaseLevel* curNode);
};


class TrunkLevel: public BaseLevel
{
public:
    float totalLen = 5.0f;                          // 总长度（整根branch的）

    float radiusMax = 0.1f;                      //
    float radiusExpandFactor = 1.2f;

    float angleTangential = 0.0f;     // 倾斜角度。

    float gravitySpinAngle = 30.0f;
    QVector<QPointF> gravityGradient_ctrlPoints, gravityGradient_splinePoints;      // 重力沿当前枝干的渐变 （UI和保存用的控制点 + 传输到3D端的光滑插值点）

    int twistNodeNum = 5;
    float twistNodeDegree = 0.05f;
    TrunkLevel(){
        this->gravityGradient_ctrlPoints = {QPointF(0.0, 0.0), QPointF(0.25, 0.0), QPointF(0.5, 0.0), QPointF(1.0, 0.0)};
        this->gravityGradient_splinePoints = CustomChartView::GetSmoothedLine(gravityGradient_ctrlPoints);
    }

    QString toString();
    static int paseTrunkLevel(QStringList lines, int StartID, TrunkLevel* curNode);

};


class BranchLevel : public TrunkLevel
{
public:
    int subBranchNum = 10;        // 这个Level在父亲branch的基础上，会分出多少枝干。
    float subStart = 0.5f;               // 这个Level在父亲branch的生成范围[subStart, subEnd]。
    float subEnd = 1.0f;

    QVector<QPointF> lenGradient_ctrlPoints, lenGradient_splinePoints;     // 沿着父亲枝干的长度渐变 （UI和保存用的控制点 + 传输到3D端的光滑插值点）


    BranchLevel(){
        this->totalLen = 3.0f; this->angleTangential = 40.0f;
        this->radiusMax = 0.060f;
        this->lenGradient_ctrlPoints = {QPointF(0.0, 0.8), QPointF(0.25, 1.0), QPointF(0.5, 1.0), QPointF(1.0, 0.4)};
        this->lenGradient_splinePoints = CustomChartView::GetSmoothedLine(lenGradient_ctrlPoints);
    }

    QString toString();
    static int paseBranchLevel(QStringList lines, int StartID, BranchLevel* curNode);

};

class LeafLevel : public BaseLevel
{
public:
    int leafNum = 10;
    float subStart = 0.4f;
    float subEnd = 1.0f;

    QString leafTextures = "_default.png";
    float leafSize = 0.8f;
    float gravityFactor = 0.0f;

    QString toString();
    static int paseLeafLevel(QStringList lines, int StartID, LeafLevel* curNode);

};

class UtilFuncs
{
public:
    static double pointToSegmentDistance(const QPointF& point, const QPointF& segmentStart, const QPointF& segmentEnd);

    static QString GradientArrayToString(QVector<QPointF>& array);

    static QVector<QPointF>ParseGradientArray(QStringList line);
};

#endif // DATASTRUCTURE_H
