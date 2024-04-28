#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// 自定义
class CircleProgress;
class MySwitchButton;
// Qt
class QNetworkAccessManager;
class QNetworkReply;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void networkInit(); // 网络通信初始化
    void layoutInit(); // 布局初始化
    void circleProgressInit(); // 进度条初始化
    void btnsInit(); // 按钮初始化
    void timerInit(); // 定时器初始化

private slots:
    void httpGet();
    void httpGetFinished(QNetworkReply *reply);
    void httpPost(QString data);
    void httpPostFinished(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;

    CircleProgress* m_humiProgress; // 湿度进度条
    CircleProgress* m_tempProgress; // 温度进度条

    MySwitchButton* m_btn1;
    MySwitchButton* m_btn2;
    MySwitchButton* m_btn3;
    MySwitchButton* m_btn4;

    QNetworkAccessManager* m_getManager; // get请求柄
    QNetworkAccessManager* m_postManager; // post请求柄
    QTimer* m_timer; // 定时请求数据
};
#endif // MAINWINDOW_H
