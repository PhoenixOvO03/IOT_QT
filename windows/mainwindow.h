#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QPushButton;
class CircleProgress;
class MySwitchButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void layoutInit(); // 布局初始化
    void circleProgressInit(); // 进度条初始化
    void btnsInit(); // 按钮初始化

public slots:
    void debugBool(bool check);

private:
    Ui::MainWindow *ui;

    QPushButton* m_add10btn; // +10
    QPushButton* m_add50btn; // +50
    QPushButton* m_sub10btn; // -10
    QPushButton* m_sub50btn; // -50
    CircleProgress* m_circleProgress; // 进度条

    MySwitchButton* m_btn1;
    MySwitchButton* m_btn2;
    MySwitchButton* m_btn3;
    MySwitchButton* m_btn4;
};
#endif // MAINWINDOW_H
