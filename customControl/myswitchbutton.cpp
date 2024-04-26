#include "myswitchbutton.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>

MySwitchButton::MySwitchButton(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(150, 50);

    // 数据初始化
    m_isOn = false;
    m_currX = height() / 2;
    m_R = height() / 2;
    m_offText = "OFF";
    m_onText = "ON";

    // 动画初始化
    m_animation = new QPropertyAnimation(this);
    m_animation->setTargetObject(this);
    m_animation->setEasingCurve(QEasingCurve::OutBounce);
    m_animation->setDirection(QVariantAnimation::Forward);
    m_animation->setDuration(500);

    // 数值变化触发重绘
    connect(m_animation, &QPropertyAnimation::valueChanged, this, [&](const QVariant& value){
        m_currX = value.toInt();
        update();
    });

    update();
}

void MySwitchButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);

    // 绘制右边进度条
    painter.setBrush(Qt::gray);
    painter.drawRoundedRect(0, 0, width(), height(), m_R, m_R);

    // 绘制左边进度条
    QLinearGradient linear(0, 0, width(), height());
    linear.setColorAt(0.2,QColor(53,179,251,150));//蓝色
    linear.setColorAt(0.8,QColor(255,88,127,200));//红色
    painter.setBrush(linear);
    painter.drawRoundedRect(0, 0, m_currX + m_R, height(), m_R, m_R);

    // 绘制按钮
    painter.setBrush(Qt::white);
    painter.drawEllipse(QPoint(m_currX, m_R), m_R, m_R);

    painter.setPen(Qt::white);
    painter.setFont(QFont("华文彩云", 30));
    if (m_currX == m_R) painter.drawText(height(), 0, width() - height(), height(), Qt::AlignCenter, m_offText);
    if (m_currX == width() - m_R) painter.drawText(0, 0, width() - height(), height(), Qt::AlignCenter, m_onText);
}

void MySwitchButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) // 左键点击
    {
        if (m_isOn) // 设置动画开始和结束数值
        {
            m_animation->setStartValue(width() - m_R);
            m_animation->setEndValue(m_R);
        }
        else
        {
            m_animation->setStartValue(m_R);
            m_animation->setEndValue(width() - m_R);
        }
        m_isOn = !m_isOn;
        emit toggled(m_isOn); // 发射更改信号
        m_animation->start(); // 开始动画
    }
}
