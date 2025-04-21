#include "hermite.h"

Hermite::Hermite()
{

}


QVector2D Hermite::GetVector2AtStep(QVector3D p1, QVector3D p2, QVector3D t1, QVector3D t2, float step)
{
    float t = step;
    QVector3D position = (2.0f * t * t * t - 3.0f * t * t + 1.0f) * p1
            + (t * t * t - 2.0f * t * t + t) * t1
            + (-2.0f * t * t * t + 3.0f * t * t) * p2
            + (t * t * t - t * t) * t2;
    return position.toVector2D();
}
