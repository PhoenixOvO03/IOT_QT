#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "circleprogress.h"
#include "myswitchbutton.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QMetaObject>
#include <QMetaMethod>

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

    layoutInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::layoutInit()
{
    circleProgressInit();
    btnsInit();

    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addWidget(m_circleProgress);
    vBox1->addWidget(m_add10btn);
    vBox1->addWidget(m_add50btn);
    vBox1->addWidget(m_sub10btn);
    vBox1->addWidget(m_sub50btn);

    QVBoxLayout* vBox2 = new QVBoxLayout();
    vBox2->addWidget(m_btn1);
    vBox2->addWidget(m_btn2);
    vBox2->addWidget(m_btn3);
    vBox2->addWidget(m_btn4);

    QHBoxLayout* hBox = new QHBoxLayout();
    hBox->addLayout(vBox1);
    hBox->addLayout(vBox2);
    ui->centralwidget->setLayout(hBox);
}

void MainWindow::circleProgressInit()
{
    m_circleProgress = new CircleProgress();
    m_add10btn = new QPushButton("+10");
    m_add50btn = new QPushButton("+50");
    m_sub10btn = new QPushButton("-10");
    m_sub50btn = new QPushButton("-50");

    connect(m_add10btn, &QPushButton::clicked, this, [&](){
        m_circleProgress->setValue(m_circleProgress->value() + 10);
    });

    connect(m_add50btn, &QPushButton::clicked, this, [&](){
        m_circleProgress->setValue(m_circleProgress->value() + 50);
    });

    connect(m_sub10btn, &QPushButton::clicked, this, [&](){
        m_circleProgress->setValue(m_circleProgress->value() - 10);
    });

    connect(m_sub50btn, &QPushButton::clicked, this, [&](){
        m_circleProgress->setValue(m_circleProgress->value() - 50);
    });
}

void MainWindow::btnsInit()
{
    m_btn1 = new MySwitchButton();
    m_btn2 = new MySwitchButton();
    m_btn3 = new MySwitchButton();
    m_btn4 = new MySwitchButton();

    connect(m_btn1, &MySwitchButton::toggled, this, &MainWindow::debugBool);
    connect(m_btn2, &MySwitchButton::toggled, this, &MainWindow::debugBool);
    connect(m_btn3, &MySwitchButton::toggled, this, &MainWindow::debugBool);
    connect(m_btn4, &MySwitchButton::toggled, this, &MainWindow::debugBool);
}

void MainWindow::debugBool(bool check)
{
    qDebug() << "按钮" << (check ? "打开" : "关闭") <<  "了";
}


