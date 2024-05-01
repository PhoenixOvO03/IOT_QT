#ifndef WEATHERINFO_H
#define WEATHERINFO_H

#include <QWidget>

class QChart;
class QChartView;
class QSplineSeries;
class QScatterSeries;
class QValueAxis;

class WeatherInfo : public QWidget
{
    Q_OBJECT

public:
    explicit WeatherInfo(QWidget *parent = nullptr);

    void setPoints(QList<QPointF> &points); // 设置图表中的点

signals:

private:
    void membersInit(); // 成员初始化

private:
    QSplineSeries* m_spLineSeries; // 折线
    QScatterSeries* m_scatterSeries; // 离散点
    QChart* m_chart; //图表
    QChartView* m_chartView; // 显示图表容器
    QValueAxis* m_axisX; // X轴坐标
    QValueAxis* m_axisY; // y轴坐标
};

#endif // WEATHERINFO_H
