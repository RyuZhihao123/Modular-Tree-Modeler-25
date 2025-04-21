#ifndef HERMITE_H
#define HERMITE_H

#include <QVector2D>
#include <QVector3D>
#include <QtCore>

class Hermite
{
public:
    Hermite();

    static QVector2D GetVector2AtStep(QVector3D p1, QVector3D p2, QVector3D t1, QVector3D t2, float step);
};

#endif // HERMITE_H
