#include "circleprogress.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

CircleProgress::CircleProgress(QWidget *parent)
    : QWidget{parent}
{
    // this->resize(200,200);
    this->setFixedSize(199, 199);

    // 初始化值
    m_outR = 99;
    m_inR = 75;
    m_value = 0;
    m_currProgress = 0;
    m_minValue = 0;
    m_maxValue = 100;
    m_name = "湿度";

    // 动画的初始化
    m_animation = new QPropertyAnimation(this);
    m_animation->setTargetObject(this);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    m_animation->setDirection(QVariantAnimation::Forward);
    m_animation->setDuration(1000);

    // 数值改变触发绘制函数
    connect(m_animation, &QPropertyAnimation::valueChanged, this, [&](const QVariant& value){
        m_currProgress = value.toInt();
        // 越界
        if (m_currProgress < m_minValue) m_currProgress = m_minValue;
        if (m_currProgress > m_maxValue) m_currProgress = m_maxValue;
        update();
    });
}

CircleProgress::CircleProgress(QString name, int minValue, int maxValue, QWidget *parent)
    : CircleProgress(parent)
{
    m_name = name;
    m_minValue = minValue;
    m_maxValue = maxValue;
}

void CircleProgress::setValue(int value)
{
    // 越界
    if (value < m_minValue) value = m_minValue;
    if (value > m_maxValue) value = m_maxValue;

    m_value = value;
    // 中止当前动画
    m_animation->stop();
    // 开始新的动画
    m_animation->setStartValue(m_currProgress);
    m_animation->setEndValue(value);
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

    outPath.arcTo(rect(), 270,  -360.0 * (m_currProgress - m_minValue) / (m_maxValue - m_minValue));
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
    painter.drawText(0, 0, width(), height(), Qt::AlignCenter, QString("%1：%2").arg(m_name).arg(m_currProgress));
    painter.restore();
}
