#ifndef MYSWITCHBUTTON_H
#define MYSWITCHBUTTON_H

#include <QWidget>

class QPropertyAnimation;

class MySwitchButton : public QWidget
{
    Q_OBJECT

public:
    enum IconType{
        WiFi,
        BlueTooth,
        Light
    };

public:
    explicit MySwitchButton(QWidget *parent = nullptr);
    explicit MySwitchButton(IconType type, QWidget *parent = nullptr);

signals:
    void toggled(bool checked);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);

private:
    void paintGray(QPainter& painter); // 绘制灰色底槽
    void paintProgress(QPainter& painter); // 绘制进度条
    void paintSwitch(QPainter& painter); // 绘制开关
    void paintText(QPainter& painter); // 绘制文字

private:
    bool m_isOn; // 是否关闭
    int m_currX; // 当前圆点位置
    int m_R; // 半径
    QString m_iconUrl; // 图标路径

    QString m_offText; // 关闭文字
    QString m_onText; // 开启文字

    QPropertyAnimation* m_animation; // 动画
};

#endif // MYSWITCHBUTTON_H
