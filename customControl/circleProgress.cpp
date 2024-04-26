#include "circleprogress.h"

#include <QPainter>
#include <QPainterPath>

CircleProgress::CircleProgress(QWidget *parent)
    : QWidget{parent}
{
    // this->resize(200,200);
    this->setFixedSize(200, 200);

    // 初始化半径和初始值
    m_outR = 100;
    m_inR = 75;
    m_value = 0;
    m_currProgress = 0;

    // 动画的初始化
    m_animation = new QPropertyAnimation(this);
    m_animation->setTargetObject(this);
    m_animation->setEasingCurve(QEasingCurve::OutElastic);
    m_animation->setDirection(QVariantAnimation::Forward);
    m_animation->setDuration(1000);

    // 数值改变触发绘制函数
    connect(m_animation, &QPropertyAnimation::valueChanged, this, [&](const QVariant& value){
        m_currProgress = value.toInt();
        if (m_currProgress < 0) m_currProgress = 0;
        if (m_currProgress > 360) m_currProgress = 360;
        update();
    });
}

void CircleProgress::setValue(int value)
{
    // 越界
    if (value > 100) value = 100;
    if (value < 0) value = 0;

    m_value = value;
    // 中止当前动画
    m_animation->stop();
    // 开始新的动画
    m_animation->setStartValue(m_currProgress);
    m_animation->setEndValue((int)(value*3.6));
    m_animation->start();
}

void CircleProgress::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    // 绘制各个部分
    paintGray(painter);
    paintProgress(painter);
    paintText(painter);
}

void CircleProgress::paintGray(QPainter &painter)
{
    painter.save();
    QPainterPath outPath; // 外圆路径
    QPainterPath inPath; // 内圆路径

    outPath.addEllipse(rect().center(), m_outR, m_outR);
    inPath.addEllipse(rect().center(), m_inR, m_inR);

    painter.setBrush(Qt::gray);
    // 大圆减去小圆实现扇形
    painter.drawPath(outPath.subtracted(inPath));
    painter.restore();
}

void CircleProgress::paintProgress(QPainter &painter)
{
    painter.save();
    QPainterPath outPath(rect().center()); // 外圆路径
    QPainterPath inPath; // 内圆路径

    outPath.arcTo(rect(), 270, -m_currProgress);
    outPath.closeSubpath();
    inPath.addEllipse(rect().center(), m_inR, m_inR);

    QConicalGradient Conical(rect().center(), 271);
    Conical.setColorAt(0.2,QColor(255,88,127,200));//红色
    Conical.setColorAt(0.8,QColor(53,179,251,150));//蓝色
    painter.setBrush(Conical);

    painter.drawPath(outPath.subtracted(inPath));
    painter.restore();
}

void CircleProgress::paintText(QPainter &painter)
{
    painter.save();
    painter.setPen(Qt::blue);
    painter.setFont(QFont("华文彩云", 20));
    painter.drawText(0, 0, width(), height(), Qt::AlignCenter, "湿度：" + QString::number((int)(m_currProgress/3.6)));
    painter.restore();
}
