#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "circleprogress.h"
#include "myswitchbutton.h"
#include "weatherinfo.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(800, 600);
    this->setWindowTitle("IOT"); // 设置标题
    this->setWindowIcon(QIcon(":/assets/IOT.png")); // 设置图标

    membersInit(); // 成员初始化
    layoutInit(); // 布局初始化

    QList<QPointF> points;
    points << QPoint(0, 10) << QPoint(1, 8) << QPoint(2, 12) << QPoint(3, 10) << QPoint(4, 5)
           << QPoint(5, 10) << QPoint(6, 8) << QPoint(7, 12) << QPoint(8, 10) << QPoint(9, 5);
    m_weather->setPoints(points);

    m_timer->start(3000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::membersInit()
{
    // 圆形进度条
    m_humiProgress = new CircleProgress("湿度", 0, 100);
    m_tempProgress = new CircleProgress("温度", -20, 50);

    // 天气信息
    m_weather = new WeatherInfo();

    // 按钮组
    m_bluetoothBtn = new MySwitchButton(MySwitchButton::BlueTooth);
    m_wifiBtn = new MySwitchButton(MySwitchButton::WiFi);
    m_lightBtn = new MySwitchButton(MySwitchButton::Light);
    m_btn4 = new MySwitchButton();
    connect(m_bluetoothBtn, &MySwitchButton::toggled, this, [&](bool check){
        QString sendMsg = QString("蓝牙：%1").arg(check ? "打开":"关闭");
        httpPost(sendMsg);
    });
    connect(m_wifiBtn, &MySwitchButton::toggled, this, [&](bool check){
        QString sendMsg = QString("wifi：%1").arg(check ? "打开":"关闭");
        httpPost(sendMsg);
    });
    connect(m_lightBtn, &MySwitchButton::toggled, this, [&](bool check){
        QString sendMsg = QString("光照：%1").arg(check ? "打开":"关闭");
        httpPost(sendMsg);
    });
    connect(m_btn4, &MySwitchButton::toggled, this, [&](bool check){
        QString sendMsg = QString("按钮4：%1").arg(check ? "打开":"关闭");
        httpPost(sendMsg);
    });

    // get post
    m_getManager = new QNetworkAccessManager();
    connect(m_getManager, &QNetworkAccessManager::finished, this, &MainWindow::httpGetFinished);
    m_postManager = new QNetworkAccessManager();
    connect(m_postManager, &QNetworkAccessManager::finished, this, &MainWindow::httpPostFinished);

    // 定时更新数据
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::httpGet);
}

void MainWindow::layoutInit()
{
    // 进度条
    QHBoxLayout* progressBox = new QHBoxLayout();
    progressBox->addWidget(m_humiProgress);
    progressBox->addSpacing(30);
    progressBox->addWidget(m_tempProgress);

    // 信息
    QVBoxLayout* infoBox = new QVBoxLayout();
    infoBox->addLayout(progressBox);
    infoBox->addWidget(m_weather);

    // 按钮群
    QVBoxLayout* btnsBox = new QVBoxLayout();
    btnsBox->addWidget(new QLabel("蓝牙"));
    btnsBox->addWidget(m_bluetoothBtn);
    btnsBox->addWidget(new QLabel("wifi"));
    btnsBox->addWidget(m_wifiBtn);
    btnsBox->addWidget(new QLabel("光照"));
    btnsBox->addWidget(m_lightBtn);
    btnsBox->addWidget(new QLabel("null"));
    btnsBox->addWidget(m_btn4);

    // 总体
    QHBoxLayout* hBox = new QHBoxLayout();
    hBox->addLayout(infoBox);
    hBox->addLayout(btnsBox);
    ui->centralwidget->setLayout(hBox);
}

void MainWindow::httpGet()
{
    QNetworkRequest quest;
    quest.setUrl(QUrl("http://127.0.0.1:12345/api/data"));
    m_getManager->get(quest);
}

void MainWindow::httpGetFinished(QNetworkReply *reply)
{
    QString allinfo = reply->readAll();
    if (allinfo.isEmpty()) return;
    qDebug() << allinfo;

    QJsonDocument json_recv = QJsonDocument::fromJson(allinfo.toUtf8());//解析json对象
    QJsonObject object = json_recv.object();
    int humi = object.value("humi").toInt();
    int temp = object.value("temp").toInt();

    m_humiProgress->setValue(humi);
    m_tempProgress->setValue(temp);
    qDebug() << humi << " " << temp;

    reply->deleteLater(); //销毁请求对象
}

void MainWindow::httpPost(QString data)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/plain"));
    request.setUrl(QUrl("http://127.0.0.1:12345/api/change"));
    m_postManager->post(request, data.toUtf8());
}

void MainWindow::httpPostFinished(QNetworkReply *reply)
{
    QString bytes = reply->readAll();
    // 获取http状态码
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
    {
        qDebug() << "status code=" << statusCode.toInt();
    }

    // 是否错误
    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError)
    {
        qDebug() << "Failed: " << reply->errorString();
    }
    else
    {
        qDebug() << bytes;
    }

    reply->deleteLater();
}
