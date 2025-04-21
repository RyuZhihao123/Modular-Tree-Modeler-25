#include "RangeSlider.h"
#include <QPainter>
#include <QMouseEvent>

RangeSlider::RangeSlider(QWidget *parent)
    : QWidget(parent),
      m_minRange(0),
      m_maxRange(100),
      m_minValue(25),
      m_maxValue(75),
      m_draggingMin(false),
      m_draggingMax(false)
{
    setMinimumHeight(30); // Ensure enough space for handles
}

void RangeSlider::setRange(int min, int max)
{
    m_minRange = min;
    m_maxRange = max;
    update();
}

void RangeSlider::setValues(int minValue, int maxValue)
{
    m_minValue = qBound(m_minRange, minValue, m_maxRange);
    m_maxValue = qBound(m_minRange, maxValue, m_maxRange);
    update();
}

void RangeSlider::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int sliderWidth = width();
    int sliderHeight = height();

    // Draw the track
    QRect trackRect(10, sliderHeight / 2 - 5, sliderWidth - 20, 5);
    painter.setBrush(Qt::lightGray);
    painter.setPen(Qt::NoPen);
    painter.drawRect(trackRect);

    // Draw the range between minValue and maxValue
    int minPos = valueToPosition(m_minValue);
    int maxPos = valueToPosition(m_maxValue);
    QRect rangeRect(minPos, sliderHeight / 2 - 5, maxPos - minPos, 5);
    painter.setBrush(Qt::blue);
    painter.drawRect(rangeRect);

    // Draw the handles
    float radiusHandle = 5;
    QRect minHandle(minPos - radiusHandle, sliderHeight / 2 - radiusHandle, radiusHandle*2, radiusHandle*2);
    QRect maxHandle(maxPos - radiusHandle, sliderHeight / 2 - radiusHandle, radiusHandle*2, radiusHandle*2);

    painter.setBrush(QColor("#63A2BB"));
    painter.drawEllipse(minHandle);
    painter.drawEllipse(maxHandle);
}

void RangeSlider::mousePressEvent(QMouseEvent *event)
{
    int clickX = event->pos().x();
    int minPos = valueToPosition(m_minValue);
    int maxPos = valueToPosition(m_maxValue);

    // Check if user clicked on min or max handle
    if (abs(clickX - minPos) < 10) {
        m_draggingMin = true;
    } else if (abs(clickX - maxPos) < 10) {
        m_draggingMax = true;
    }
}

void RangeSlider::mouseMoveEvent(QMouseEvent *event)
{
    int mouseX = event->pos().x();

    if (m_draggingMin) {
        int newValue = positionToValue(mouseX);
        if (newValue < m_maxValue) {
            m_minValue = qMax(m_minRange, newValue);
            emit rangeChanged(m_minValue, m_maxValue);
        }
    } else if (m_draggingMax) {
        int newValue = positionToValue(mouseX);
        if (newValue > m_minValue) {
            m_maxValue = qMin(m_maxRange, newValue);
            emit rangeChanged(m_minValue, m_maxValue);
        }
    }

    update();
}

int RangeSlider::valueToPosition(int value) const
{
    int range = m_maxRange - m_minRange;
    int sliderWidth = width() - 20; // Account for margins
    return 10 + ((value - m_minRange) * sliderWidth) / range;
}

int RangeSlider::positionToValue(int position) const
{
    int range = m_maxRange - m_minRange;
    int sliderWidth = width() - 20; // Account for margins
    return m_minRange + ((position - 10) * range) / sliderWidth;
}
