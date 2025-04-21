#include "datastructure.h"
#include <QtMath>


double UtilFuncs::pointToSegmentDistance(const QPointF &point, const QPointF &segmentStart, const QPointF &segmentEnd)
{
    // 线段起点和终点向量
    double dx = segmentEnd.x() - segmentStart.x();
    double dy = segmentEnd.y() - segmentStart.y();

    // 线段长度的平方
    double lengthSquared = dx * dx + dy * dy;

    if (lengthSquared == 0.0)
    {
        // 如果线段是一个点，返回点到该点的距离
        return qSqrt(qPow(point.x() - segmentStart.x(), 2) + qPow(point.y() - segmentStart.y(), 2));
    }

    // 投影比例 t
    double t = ((point.x() - segmentStart.x()) * dx + (point.y() - segmentStart.y()) * dy) / lengthSquared;

    // 限制 t 的范围在 [0, 1]
    t = qMax(0.0, qMin(1.0, t));

    // 投影点坐标
    QPointF projection(segmentStart.x() + t * dx, segmentStart.y() + t * dy);

    // 返回点到投影点的距离
    return qSqrt(qPow(point.x() - projection.x(), 2) + qPow(point.y() - projection.y(), 2));
}


QString UtilFuncs::GradientArrayToString(QVector<QPointF>& array)
{
    QString content = "";
    for(int i=0; i<array.size(); ++i)
        content += QString::number(array[i].x()) + " " +QString::number(array[i].y()) +" ";
    return content;
}

QVector<QPointF> UtilFuncs::ParseGradientArray(QStringList line)
{
    QVector<QPointF> array;

    for(int i=1; i<line.size(); i+=2)
    {
        int id0 = i;
        int id1 = i + 1;

        if (line[id0] == "" || id1 >= line.size() || line[id1] == "")
            break;

        array.append(QPointF(line[id0].toFloat(), line[id1].toFloat()));
    }
    return array;
}




///
/// BaseNode toString
///
QString BaseLevel::toString()
{
    QString content = "+++ "+this->type +"\n";
    content += "name "+this->name +"\n";
    content += "type "+this->type +"\n";
    content += "seed "+QString::number(this->seed) +"\n";
    content += "pos " + QString::number(this->pos.x()) +" " + QString::number(this->pos.y())+"\n";

    content += "---\n";
    return content;
}

int BaseLevel::paseBaseLevel(QStringList lines, int StartID, BaseLevel* curNode)
{
    int i = StartID;

    while (true)
    {
        QStringList items = lines[i].split(QRegExp("\\s+"));

        if (items.size() == 0 || items[0] == "---")  // 结束
        {
            i++;
            break;
        }

        if (items[0] == "name") curNode->name = items[1];
        if (items[0] == "type") curNode->type = items[1];
        if (items[0] == "seed") curNode->seed = items[1].toInt();
        if (items[0] == "pos") curNode->pos = QPoint(items[1].toInt(), items[2].toInt());
        i++;
    }

    return i;  // 返回下一行的行号ID.
}

QString TrunkLevel::toString()
{
    QString content = BaseLevel::toString();  // 调用基类函数。
    content += "totalLen "+QString::number(this->totalLen) +"\n";
    content += "radiusMax "+QString::number(this->radiusMax) +"\n";
    content += "radiusExpandFactor "+QString::number(this->radiusExpandFactor) +"\n";
    content += "angleTangential "+QString::number(this->angleTangential) +"\n";
    content += "gravitySpinAngle "+QString::number(this->gravitySpinAngle) +"\n";

    content += "gravityGradientCtrlPts " + UtilFuncs::GradientArrayToString(this->gravityGradient_ctrlPoints) + "\n";

    content += "gravityGradient ";
    for(int i=0; i<gravityGradient_splinePoints.size(); ++i)
        content += QString::number(gravityGradient_splinePoints[i].x()) + " " +QString::number(gravityGradient_splinePoints[i].y()) +" ";
    content+= "\n";

    content += "twistNodeProperty " + QString::number(this->twistNodeNum) +" " + QString::number(twistNodeDegree) + "\n";

    content += "---\n";
    return content;
}

int TrunkLevel::paseTrunkLevel(QStringList lines, int StartID, TrunkLevel *curNode)
{
    int i = paseBaseLevel(lines, StartID, curNode);  // 当前行号ID。
    while (true)
    {
        QStringList items = lines[i].split(QRegExp("\\s+"));
        if (items.size() == 0 || items[0] == "---")  // 结束
        {
            i++;  break;
        }
        if (items[0] == "totalLen") curNode->totalLen = items[1].toFloat();
        if (items[0] == "radiusMax") curNode->radiusMax = items[1].toFloat();
        if (items[0] == "radiusExpandFactor") curNode->radiusExpandFactor = items[1].toFloat();
        if (items[0] == "angleTangential") curNode->angleTangential = items[1].toFloat();
        if (items[0] == "gravitySpinAngle") curNode->gravitySpinAngle = items[1].toFloat();
        if (items[0] == "gravityGradientCtrlPts") curNode->gravityGradient_ctrlPoints = UtilFuncs::ParseGradientArray(items);
        if (items[0] == "gravityGradient") curNode->gravityGradient_splinePoints = UtilFuncs::ParseGradientArray(items);
        if (items[0] == "twistNodeProperty") curNode->twistNodeNum = items[1].toInt(), curNode->twistNodeDegree = items[2].toFloat();

        i++;
    }
    return i;  // 返回下一行的行号ID.
}

QString BranchLevel::toString()
{
    QString content = TrunkLevel::toString();  // 调用基类函数。
    content += "subBranchNum "+QString::number(this->subBranchNum) +"\n";
    content += "subStart "+QString::number(this->subStart) +"\n";
    content += "subEnd "+QString::number(this->subEnd) +"\n";

    content += "lenGradientCtrlPts "+UtilFuncs::GradientArrayToString(this->lenGradient_ctrlPoints) +"\n";

    content += "lenGradient ";
    for(int i=0; i<lenGradient_splinePoints.size(); ++i)
        content += QString::number(lenGradient_splinePoints[i].x()) + " " +QString::number(lenGradient_splinePoints[i].y()) +" ";
    content+= "\n";

    content += "---\n";
    return content;
}


int BranchLevel::paseBranchLevel(QStringList lines, int StartID, BranchLevel *curNode)
{
    int i = paseTrunkLevel(lines, StartID, curNode);  // 当前行号ID。
    while (true)
    {
        QStringList items = lines[i].split(QRegExp("\\s+"));
        if (items.size() == 0 || items[0] == "---")  // 结束
        {
            i++;  break;
        }
        if (items[0] == "subBranchNum") curNode->subBranchNum = items[1].toInt();
        if (items[0] == "subStart") curNode->subStart = items[1].toFloat();
        if (items[0] == "subEnd") curNode->subEnd = items[1].toFloat();
        if (items[0] == "lenGradientCtrlPts") curNode->lenGradient_ctrlPoints = UtilFuncs::ParseGradientArray(items);
        if (items[0] == "lenGradient") curNode->lenGradient_splinePoints = UtilFuncs::ParseGradientArray(items);

        i++;
    }
    return i;  // 返回下一行的行号ID.
}

QString LeafLevel::toString()
{
    QString content = BaseLevel::toString();  // 调用基类函数。
    content += "leafNum "+QString::number(this->leafNum) +"\n";
    content += "subStart "+QString::number(this->subStart) +"\n";
    content += "subEnd "+QString::number(this->subEnd) +"\n";

    content += "leafTextures "+this->leafTextures +"\n";
    content += "leafSize "+QString::number(this->leafSize) +"\n";
    content += "gravityFactor "+QString::number(this->gravityFactor) +"\n";

    content += "---\n";
    return content;
}


int LeafLevel::paseLeafLevel(QStringList lines, int StartID, LeafLevel *curNode)
{
    int i = paseBaseLevel(lines, StartID, curNode);  // 当前行号ID。
    while (true)
    {
        QStringList items = lines[i].split(QRegExp("\\s+"));
        if (items.size() == 0 || items[0] == "---")  // 结束
        {
            i++;  break;
        }
        if (items[0] == "leafNum") curNode->leafNum = items[1].toInt();
        if (items[0] == "subStart") curNode->subStart = items[1].toFloat();
        if (items[0] == "subEnd") curNode->subEnd = items[1].toFloat();

        if (items[0] == "leafTextures") curNode->leafTextures = items[1];
        if (items[0] == "leafSize") curNode->leafSize = items[1].toFloat();
        if (items[0] == "gravityFactor") curNode->gravityFactor = items[1].toFloat();
        i++;
    }
    return i;  // 返回下一行的行号ID.
}
