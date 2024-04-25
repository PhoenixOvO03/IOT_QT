#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "circleprogress.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

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

    m_dash = new CircleProgress();

    m_add10btn = new QPushButton("+10");
    m_add50btn = new QPushButton("+50");
    m_sub10btn = new QPushButton("-10");
    m_sub50btn = new QPushButton("-50");
    QVBoxLayout* vBox = new QVBoxLayout();

    vBox->addWidget(m_add10btn);
    vBox->addWidget(m_add50btn);
    vBox->addWidget(m_sub10btn);
    vBox->addWidget(m_sub50btn);

    QHBoxLayout* hBox = new QHBoxLayout();
    hBox->addWidget(m_dash);
    hBox->addLayout(vBox);
    ui->centralwidget->setLayout(hBox);

    connect(m_add10btn, &QPushButton::clicked, this, [&](){
        m_dash->setValue(m_dash->value() + 10);
    });

    connect(m_add50btn, &QPushButton::clicked, this, [&](){
        m_dash->setValue(m_dash->value() + 50);
    });

    connect(m_sub10btn, &QPushButton::clicked, this, [&](){
        m_dash->setValue(m_dash->value() - 10);
    });

    connect(m_sub50btn, &QPushButton::clicked, this, [&](){
        m_dash->setValue(m_dash->value() - 50);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
