#ifndef PARAMETERPANEL_H
#define PARAMETERPANEL_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

namespace Ui {
class ParameterPanel;
}

class ParameterPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterPanel(QWidget *parent = 0);
    ~ParameterPanel();

    QRect imgRect;
    QImage m_backgroundImg;
    QPointF m_curCursorPos;
    QPointF m_gbValue;
    bool showText;
    void initBackground();
    void pseudoColorImage();
    QPointF xyTogb(float x, float y);
    QPointF gbToxy(float g, float b);


    void setGB(float g, float b){m_gbValue = QPointF(g,b); m_curCursorPos = gbToxy(g,b); update();}
    QPointF flipAtBorder(const QPointF& globalPos);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    Ui::ParameterPanel *ui;
};

#endif // PARAMETERPANEL_H
