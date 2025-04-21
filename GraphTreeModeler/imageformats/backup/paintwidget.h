#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QStack>
#include "datastructure.h"

namespace Ui {
class PaintWidget;
}

enum SketchMode{
    _SKELETON,
    _FOLIAGE
};

class PaintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = 0);
    ~PaintWidget();

    bool m_isAutoAlign = false;

    QList<BaseLevel*> m_baselevels;
    QMap<BaseLevel*, QList<BaseLevel* > > m_edges;

    QString currentAddNodeType = "TrunkLevel";
    BaseLevel* m_curPtr = nullptr;  // 选择顶点时的情况。

    BaseLevel *m_curEdgeA = nullptr, *m_curEdgeB = nullptr;  // 选择边的情况

    void ClearAll(bool isCreatedRoot);

    void CleanConnectionGraph();   // 将graph的拓扑变成合理的链接。
    void SaveConstructedTreeStructure(const QString &filenameNode, const QString &filenameEdge);
    void LoadConstructedTreeStructure(const QString &filenameNode, const QString &filenameEdge);
protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;

    void paintEvent(QPaintEvent*e) override;  // 绘制


private:
    Ui::PaintWidget *ui;

    bool isDrawLine = false;
    QPoint m_curMousePos;
    BaseLevel *CreateNewNode(QPoint pos);
    QString GetPrintName(BaseLevel *&node);

signals:
    void sig_selectedChanged();
};

#endif // PAINTWIDGET_H
