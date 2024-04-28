#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "circleprogress.h"
#include "myswitchbutton.h"

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
    this->resize(400,400);

    // 设置标题
    this->setWindowTitle("IOT");
    // 设置图标
    this->setWindowIcon(QIcon(":/assets/IOT.png"));

    networkInit(); // 网络通信初始化
    circleProgressInit(); // 进度条初始化
    btnsInit(); // 按钮组初始化
    layoutInit(); // 布局初始化
    timerInit(); // 定时器初始化
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::networkInit()
{
    // get
    m_getManager = new QNetworkAccessManager();
    connect(m_getManager, &QNetworkAccessManager::finished, this, &MainWindow::httpGetFinished);

    // post
    m_postManager = new QNetworkAccessManager();
    connect(m_postManager, &QNetworkAccessManager::finished, this, &MainWindow::httpPostFinished);
}

void MainWindow::layoutInit()
{
    // 进度条
    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addWidget(m_humiProgress);
    vBox1->addWidget(m_tempProgress);

    // 按钮群
    QVBoxLayout* vBox2 = new QVBoxLayout();
    vBox2->addWidget(new QLabel("按钮1"));
    vBox2->addWidget(m_btn1);
    vBox2->addWidget(new QLabel("按钮2"));
    vBox2->addWidget(m_btn2);
    vBox2->addWidget(new QLabel("按钮3"));
    vBox2->addWidget(m_btn3);
    vBox2->addWidget(new QLabel("按钮4"));
    vBox2->addWidget(m_btn4);

    // 总体
    QHBoxLayout* hBox = new QHBoxLayout();
    hBox->addLayout(vBox1);
    hBox->addLayout(vBox2);
    ui->centralwidget->setLayout(hBox);
}

void MainWindow::circleProgressInit()
{
    m_humiProgress = new CircleProgress("湿度", 0, 100);
    m_tempProgress = new CircleProgress("温度", -20, 50);
}

void MainWindow::btnsInit()
{
    m_btn1 = new MySwitchButton();
    m_btn2 = new MySwitchButton();
    m_btn3 = new MySwitchButton();
    m_btn4 = new MySwitchButton();

    connect(m_btn1, &MySwitchButton::toggled, this, [&](bool check){
        QString sendMsg = QString("按钮1：%1").arg(check ? "打开":"关闭");
        httpPost(sendMsg);
    });

    connect(m_btn2, &MySwitchButton::toggled, this, [&](bool check){
        QString sendMsg = QString("按钮2：%1").arg(check ? "打开":"关闭");
        httpPost(sendMsg);
    });

    connect(m_btn3, &MySwitchButton::toggled, this, [&](bool check){
        QString sendMsg = QString("按钮3：%1").arg(check ? "打开":"关闭");
        httpPost(sendMsg);
    });

    connect(m_btn4, &MySwitchButton::toggled, this, [&](bool check){
        QString sendMsg = QString("按钮4：%1").arg(check ? "打开":"关闭");
        httpPost(sendMsg);
    });
}

void MainWindow::timerInit()
{
    // 定时更新数据
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::httpGet);
    m_timer->start(3000);
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
