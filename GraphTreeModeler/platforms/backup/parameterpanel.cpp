#include "parameterpanel.h"
#include "ui_parameterpanel.h"
#include <QDebug>
#include <cmath>

ParameterPanel::ParameterPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParameterPanel)
{
    ui->setupUi(this);
//    this->imgRect = QRect(30,30,this->width()-30, this->height()-30);
    this->imgRect = QRect(60,30,this->width()-30, this->height()-30);
    this->m_curCursorPos = QPoint(imgRect.x(), imgRect.y()+imgRect.height()/2);
    this->m_gbValue = xyTogb(this->m_curCursorPos.x(), this->m_curCursorPos.y());
    this->initBackground();
}

void ParameterPanel::initBackground()
{
    this->m_backgroundImg = QImage(imgRect.width(), imgRect.height(), QImage::Format_RGB32);
    this->pseudoColorImage();
}

void ParameterPanel::pseudoColorImage()
{
    for(int i = 0;i < this->imgRect.width();i++)
        for(int j = 0;j < this->imgRect.height();j++)
        {
            float gravity = fabs((imgRect.height()/2.0f - j) / (imgRect.height()/2.0f));  // 0,1
            float blue = i / (float)imgRect.width();  // 0 1

            float grey = (gravity+blue) /2.0f *255.0f;
            float r,g,b;
            if(0 <= grey && grey < 64)
            {
                r = 0;
                g = 4*grey;
                b = 255;
            }
            else if(64 <= grey && grey < 128)
            {
                r = 0;
                g = 255;
                b = -4*(grey-64)+255.0;
            }
            else if(127 <= grey && grey <= 192)
            {
                r = 4*(grey-128);
                g = 255;
                b = 0;
            }
            else
            {
                r = 255;
                g = -4*(grey-255);
                b = 0;
            }

            this->m_backgroundImg.setPixelColor(i, j, QColor(r,g,b));
        }
}

// mouseevent event->pos()
QPointF ParameterPanel::xyTogb(float x, float y)
{
    float g = 1.0 - (y-imgRect.y()) / imgRect.height()*2;
    float b = (x - imgRect.x()) / imgRect.width();
    return QPointF(g, b);
}

QPointF ParameterPanel::gbToxy(float g, float b)
{
    float x = imgRect.x() + b * imgRect.width();
    float y = imgRect.y() + (1-g) * imgRect.height()/2.0f;
    return QPointF(x, y);
}

void ParameterPanel::paintEvent(QPaintEvent *event)
{
    float cursorRadius = 6.0f;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawImage(this->imgRect, this->m_backgroundImg);


    painter.setBrush(QBrush(QColor(255,100,155)));
    painter.setBrush(QBrush(QColor(132,131,199)));
    painter.drawEllipse(this->m_curCursorPos, cursorRadius, cursorRadius);

    painter.setBrush(QBrush(QColor(100,200,100)));
    painter.setPen(QColor(192,0,0));
    painter.setPen(QColor(141,81,81));
    painter.setFont(QFont("Meiryo",9,QFont::Bold));
    //painter.drawEllipse(this->m_curCursorPos, cursorRadius, cursorRadius);

    painter.drawText(this->width()/2.7f,20,
                     QString("Base-Dir"));
    painter.setPen(QColor(25,50,50));

    painter.drawText(this->width()/2.7f+60,20,
                     QString("(%1)").arg(this->m_gbValue.y(),0, 'f', 3));
    //painter.drawText(0, 20, this->width(),30,Qt::AlignCenter, QString("BaseDir\n %1").arg(this->m_gbValue.y()));
    painter.save();//显将画笔属性进行保存
    painter.translate(20,100);//将原点移动到要画文字的地方
    painter.rotate(-90);//将画笔翻转90度
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QColor(192,0,0));
    painter.setFont(QFont("Meiryo",9,QFont::Bold));

//    painter.drawText(0,0,QString("Gravity\n %2").arg(this->m_gbValue.x()));//绘制文字内容
//    painter.drawText(0, 0, this->height()*0.5,40,Qt::AlignCenter,
//                     QString("Gravity\n %1").arg(this->m_gbValue.x(),0, 'f', 3));
    painter.setPen(QColor(141,81,81));
    painter.drawText(0, 0, this->height()*0.5,30,Qt::AlignCenter,
                     QString("Gravity"));
        painter.setPen(QColor(25,50,50));
    painter.drawText(0, 15, this->height()*0.5,30,Qt::AlignCenter,
                     QString("(%1)").arg(this->m_gbValue.x(),0, 'f', 3));
    painter.restore();//恢复画笔原有属性


    if(this->showText)
    {
        float w = 120;
        float h = 30;
        float x = m_curCursorPos.x()-w/2;
        float y = m_curCursorPos.y();


        x = x<imgRect.left()?imgRect.left():x;
        x = x>(imgRect.right()-w)?(imgRect.right()-w):x;
        y = y>(imgRect.bottom()-h/2) ? (imgRect.bottom()-h/2):y;
        QRect rectangle = QRect(x, y, w, h);
        painter.setBrush(QBrush(QColor(200,200,200,50)));
        painter.setPen(Qt::NoPen);
        painter.drawRect(rectangle);
        painter.setPen(QPen(QColor(50,80,80,200)));
        QString g_str = QString::number(this->m_gbValue.x(), 'f', 2);
        QString b_str = QString::number(this->m_gbValue.y(), 'f', 2);
        QString gb_str = b_str + ", " + g_str;
        painter.drawText(rectangle, Qt::AlignLeft, gb_str);
    }
}

QPointF ParameterPanel::flipAtBorder(const QPointF &globalPos)
{
    QPointF res = globalPos;

    res.setX( res.x() <imgRect.x()? imgRect.x(): res.x());
    res.setY( res.y() <imgRect.y()? imgRect.y(): res.y());
    res.setX( res.x() >imgRect.bottomRight().x()? imgRect.bottomRight().x(): res.x());
    res.setY( res.y() >imgRect.bottomRight().y()? imgRect.bottomRight().y(): res.y());
    return res;
}

void ParameterPanel::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint cur = event->pos();

        this->m_curCursorPos = this->flipAtBorder(cur);
        this->m_gbValue = xyTogb(m_curCursorPos.x(), m_curCursorPos.y());
        this->showText = true;


        this->update();
    }
}

void ParameterPanel::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QPoint cur = event->pos();

        this->m_curCursorPos = flipAtBorder(cur);
        this->m_gbValue = xyTogb(m_curCursorPos.x(), m_curCursorPos.y());
        this->update();

        //qDebug()<<m_curCursorPos << m_gbValue << gbToxy(m_gbValue.x(), m_gbValue.y());
    }
}

void ParameterPanel::mouseReleaseEvent(QMouseEvent *event)
{
    this->showText = false;
    this->update();
}
//添加一个QLabel控件，鼠标点击时出现，不点击隐藏，可以跟随鼠标移动，显示坐标数值，保留小数点后2位
ParameterPanel::~ParameterPanel()
{
    delete ui;
}
