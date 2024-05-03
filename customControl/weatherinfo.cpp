#include "weatherinfo.h"

#include <QChartView>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QValueAxis>

WeatherInfo::WeatherInfo(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(400, 200);
    membersInit();

    QList<QPointF> points;
    points << QPoint(0, 0) << QPoint(1, 1) << QPoint(2, 2) << QPoint(3, 3) << QPoint(4, 4)
           << QPoint(5, 5) << QPoint(6, 6) << QPoint(7, 7) << QPoint(8, 8) << QPoint(9, 9);
    setPoints(points);
}

void WeatherInfo::setPoints(QList<QPointF> &points)
{
    // 清空当前所有点
    m_spLineSeries->clear();
    m_scatterSeries->clear();
    // 添加数据
    m_spLineSeries->append(points);
    m_scatterSeries->append(points);

    int minX = INT_MAX;
    int minY = INT_MAX;
    int maxX = INT_MIN;
    int maxY = INT_MIN;

    // 寻找极值
    for (const QPointF& point : points)
    {
        if (minX > point.x()) minX = point.x();
        if (minY > point.y()) minY = point.y();
        if (maxX < point.x()) maxX = point.x();
        if (maxY < point.y()) maxY = point.y();
    }
    // 当前显示范围扩大一圈，让图显示完整
    m_axisX->setRange(minX-(maxX-minX)/10.0, maxX+(maxX-minX)/10.0);
    m_axisY->setRange(minY-(maxY-minY)/10.0, maxY+(maxY-minY)/4.0);
}

void WeatherInfo::membersInit()
{
    // 曲线
    m_spLineSeries = new QSplineSeries();
    // 离散点
    m_scatterSeries = new QScatterSeries();
    m_scatterSeries->setBrush(QColor(255,192,203)); // 内部颜色
    m_scatterSeries->setPen(QPen(QColor(53,179,251), 3)); // 边框
    m_scatterSeries->setPointLabelsFormat("@yPoint"); // 显示y值
    m_scatterSeries->setPointLabelsColor(QColor(Qt::blue)); // 标签字体颜色
    m_scatterSeries->setPointLabelsFont(QFont("黑体")); // 标签字体
    m_scatterSeries->setPointLabelsVisible(true); // 设置标签可见性

    // 图表
    m_chart = new QChart();
    m_chart->addSeries(m_spLineSeries); // 添加曲线
    m_chart->addSeries(m_scatterSeries); // 添加离散点
    m_chart->legend()->hide(); // 不显示系列名称了
    m_chart->setBackgroundVisible(false); // 隐藏背景

    // X坐标
    m_axisX = new QValueAxis();
    m_chart->addAxis(m_axisX,Qt::AlignBottom);
    m_axisX->setVisible(false);
    m_spLineSeries->attachAxis(m_axisX);
    m_scatterSeries->attachAxis(m_axisX);
    // Y坐标
    m_axisY = new QValueAxis();
    m_chart->addAxis(m_axisY,Qt::AlignLeft);
    m_axisY->setVisible(false);
    m_spLineSeries->attachAxis(m_axisY);
    m_scatterSeries->attachAxis(m_axisY);

    m_chartView = new QChartView(this); // 显示图表容器
    m_chartView->setChart(m_chart); // 向chartView中添加图表
    m_chartView->resize(QSize(width(),height())); // 重新设置chartView的大小
    m_chartView->setRenderHints(QPainter::Antialiasing); // 抗锯齿
    // 渐变背景
    QLinearGradient linear(QPoint(width()/2,0), QPoint(width()/2, height()));
    linear.setColorAt(0.2,QColor(255,88,127,100)); //红色
    linear.setColorAt(0.8,QColor(53,179,251,100)); //蓝色
    m_chartView->setBackgroundBrush(linear);
}
