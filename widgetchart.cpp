#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>
#include <QFileDialog>
#include "widgetchart.h"


WidgetChart::WidgetChart(QWidget* parent)
 : QWidget(parent)
{
    chartView = new QChartView();

    switch (typeWidget)
    {
    case TypeWidgetChart::Area :
        createAreaChart();
        break;
    case TypeWidgetChart::Bar :
        createBarChart();
        break;
    case TypeWidgetChart::Line :
        createLineChart();
        break;
    case TypeWidgetChart::Pie :
        createPieChart();
        break;
    case TypeWidgetChart::Spline :
        createSplineChart();
        break;
    case TypeWidgetChart::Scatter :
        createScatterChart();
        break;
    }
}

DataTable WidgetChart::generateRandomData(int listCount, int valueMax, int valueCount)
{
    DataTable dataTable;

    // set seed for random stuff
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    // generate random data
    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + (qreal)(qrand() % valueMax) / (qreal) valueCount;
            QPointF value((j + (qreal) rand() / (qreal) RAND_MAX) * ((qreal) valueMax / (qreal) valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}

void WidgetChart::createAreaChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Area chart");

    // The lower series initialized to zero values
    QLineSeries *lowerSeries = nullptr;
    QString name("Series ");
    int nameIndex = 0;
    for (int i(0); i < m_dataTable.count(); i++) {
        QLineSeries *upperSeries = new QLineSeries(chart);
        for (int j(0); j < m_dataTable[i].count(); j++) {
            Data data = m_dataTable[i].at(j);
            if (lowerSeries) {
                const QVector<QPointF>& points = lowerSeries->pointsVector();
                upperSeries->append(QPointF(j, points[i].y() + data.first.y()));
            } else {
                upperSeries->append(QPointF(j, data.first.y()));
            }
        }
        QAreaSeries *area = new QAreaSeries(upperSeries, lowerSeries);
        area->setName(name + QString::number(nameIndex));

        if (notColored)
        {
            QLinearGradient gradient(0, 0, this->width(), this->height());
            gradient.setColorAt(0, Qt::black);
            gradient.setColorAt(1, Qt::white);

            QBrush brush(gradient);
            area->setBrush(brush);
        }
        nameIndex++;
        chart->addSeries(area);
        chart->createDefaultAxes();
        lowerSeries = upperSeries;
    }

    chartView->setChart(chart);
}

void WidgetChart::createBarChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Bar chart");

    QStackedBarSeries *series = new QStackedBarSeries(chart);
    for (int i(0); i < m_dataTable.count(); i++) {
        QBarSet *set = new QBarSet("Bar set " + QString::number(i));
        for (const Data &data : m_dataTable[i])
            *set << data.first.y();

        if (notColored)
        {
            QLinearGradient gradient(0, 0, this->width(), this->height());
            gradient.setColorAt(0, Qt::black);
            gradient.setColorAt(1, Qt::white);

            QBrush brush(gradient);
            set->setBrush(brush);
        }
        series->append(set);
    }
    chart->addSeries(series);
    chart->createDefaultAxes();

    chartView->setChart(chart);
}

void WidgetChart::createLineChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Line chart");

    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QLineSeries *series = new QLineSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        if (notColored)
        {
            QPen pen(Qt::black);
            series->setPen(pen);
        }
        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();

    chartView->setChart(chart);
}

void WidgetChart::createPieChart() const
{
    QChart *chart = new QChart();
    chart->setTitle("Pie chart");

    qreal pieSize = 1.0 / m_dataTable.count();
    for (int i = 0; i < m_dataTable.count(); i++) {
        QPieSeries *series = new QPieSeries(chart);
        for (const Data &data : m_dataTable[i]) {
            QPieSlice *slice = series->append(data.second, data.first.y());
            if (data == m_dataTable[i].first()) {
                slice->setLabelVisible();
                slice->setExploded();
            }

            if (notColored)
            {
                //QConicalGradient gradient(chartView->height() / 2, chartView->width() / 2, 90);
                QLinearGradient gradient(0, 0, chartView->height(), chartView->width());
                gradient.setColorAt(0, Qt::black);
                gradient.setColorAt(1, Qt::white);

                QBrush brush(gradient);
                slice->setBrush(brush);
            }
        }
        qreal hPos = (pieSize / 2) + (i / (qreal) m_dataTable.count());
        series->setPieSize(pieSize);
        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.5);
        chart->addSeries(series);
    }

    chartView->setChart(chart);
}

void WidgetChart::createSplineChart() const
{
    // spine chart
    QChart *chart = new QChart();
    chart->setTitle("Spline chart");
    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QSplineSeries *series = new QSplineSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));

        if (notColored)
        {
            QPen pen(Qt::black);
            series->setPen(pen);
        }

        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    chartView->setChart(chart);
}

void WidgetChart::createScatterChart() const
{
    // scatter chart
    QChart *chart = new QChart();
    chart->setTitle("Scatter chart");
    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QScatterSeries *series = new QScatterSeries(chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));

        if (notColored)
        {
            QLinearGradient gradient(0, 0, chartView->height(), chartView->width());
            gradient.setColorAt(0, Qt::black);
            gradient.setColorAt(1, Qt::white);

            QBrush brush(gradient);
            series->setBrush(brush);
        }

        nameIndex++;
        chart->addSeries(series);
    }
    chart->createDefaultAxes();
    chartView->setChart(chart);
}

void WidgetChart::updateWidget(TypeWidgetChart newType, bool notColoredChart)
{
    typeWidget = newType;
    notColored = notColoredChart;

    switch (typeWidget)
    {
    case TypeWidgetChart::Area :
        createAreaChart();
        break;
    case TypeWidgetChart::Bar :
        createBarChart();
        break;
    case TypeWidgetChart::Line :
        createLineChart();
        break;
    case TypeWidgetChart::Pie :
        createPieChart();
        break;
    case TypeWidgetChart::Spline :
        createSplineChart();
        break;
    case TypeWidgetChart::Scatter :
        createScatterChart();
        break;
    }
}

