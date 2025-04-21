#ifndef RANGESLIDER_H
#define RANGESLIDER_H

#include <QWidget>

class RangeSlider : public QWidget
{
    Q_OBJECT

public:
    explicit RangeSlider(QWidget *parent = nullptr);
    void setRange(int min, int max);
    void setValues(int minValue, int maxValue);

    int minValue() const { return m_minValue; }
    int maxValue() const { return m_maxValue; }

signals:
    void rangeChanged(int minValue, int maxValue);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    int m_minRange;
    int m_maxRange;
    int m_minValue;
    int m_maxValue;

    bool m_draggingMin;
    bool m_draggingMax;

    int valueToPosition(int value) const;
    int positionToValue(int position) const;
};

#endif // RANGESLIDER_H
