#ifndef CIRCLEPROGRESS_H
#define CIRCLEPROGRESS_H

#include <QWidget>
#include <QPropertyAnimation>

class QPropertyAnimation;

class CircleProgress : public QWidget
{
    Q_OBJECT
public:
    explicit CircleProgress(QWidget *parent = nullptr);

    void setValue(int value);
    inline int value(){return m_value;}

protected:
    void paintEvent(QPaintEvent *event);

signals:

private:
    void paintGray(QPainter& painter); // 绘制灰色底槽
    void paintProgress(QPainter& painter); // 绘制进度条
    void paintText(QPainter& painter); // 绘制中心文字

private:
    int m_value; // 目标值
    int m_currProgress; // 当前动画值
    int m_outR; // 外圆半径
    int m_inR; // 内圆半径

    QPropertyAnimation* m_animation;    // 动画
};

#endif // CIRCLEPROGRESS_H
