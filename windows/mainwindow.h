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
class WeatherInfo;
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
    void membersInit(); // 成员初始化
    void layoutInit(); // 布局初始化

private slots:
    void getTempHumi(); // 获取温湿度
    void getTempHumiFinished(QNetworkReply *reply);
    void getTempArray(); // 获取温度数组
    void getTempArrayFinished(QNetworkReply *reply);
    void httpPostString(QString data); // 发送字符串
    void httpPostStringFinished(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;

    CircleProgress* m_humiProgress; // 湿度进度条
    CircleProgress* m_tempProgress; // 温度进度条

    WeatherInfo* m_weather; // 天气面板

    MySwitchButton* m_bluetoothBtn; // 蓝牙按钮
    MySwitchButton* m_wifiBtn; // wifi按钮
    MySwitchButton* m_lightBtn; // 光照按钮
    MySwitchButton* m_btn4;

    QNetworkAccessManager* m_getTempHumiManager; // get温湿度请求柄
    QNetworkAccessManager* m_getTempArrayManager; // get温度数组
    QNetworkAccessManager* m_postStringManager; // post字符串请求柄
    QTimer* m_timer; // 定时请求数据
};
#endif // MAINWINDOW_H
