#include "printchart.h"

void PrinterChartBar::createChart(QChartView &chartView, const DataTable& data, bool notColored)
{
    QChart *chart = new QChart();
    chart->setTitle("Bar chart");

    QStackedBarSeries *series = new QStackedBarSeries(chart);
    for (int i(0); i < data.count(); i++) {
        QBarSet *set = new QBarSet("Bar set " + QString::number(i));
        for (const Data &data : data[i])
            *set << data.first.y();

        if (notColored)
        {
            QLinearGradient gradient(0, 0, 100, 100);
            gradient.setColorAt(0, Qt::black);
            gradient.setColorAt(1, Qt::white);

            QBrush brush(gradient);
            set->setBrush(brush);
        }
        series->append(set);
    }
    chart->addSeries(series);
    chart->createDefaultAxes();

    chartView.setChart(chart);
}

void PrinterChartPie::createChart(QChartView &chartView, const DataTable& dataTable, bool notColored)
{
    QChart *chart = new QChart();
    chart->setTitle("Pie chart");

    qreal pieSize = 1.0 / dataTable.count();
    for (int i = 0; i < dataTable.count(); i++) {
        QPieSeries *series = new QPieSeries(chart);
        for (const Data &data : dataTable[i]) {
            QPieSlice *slice = series->append(data.second, data.first.y());
            if (data == dataTable[i].first()) {
                slice->setLabelVisible();
                slice->setExploded();
            }

            if (notColored)
            {
                //QConicalGradient gradient(chartView->height() / 2, chartView->width() / 2, 90);
                QLinearGradient gradient(0, 0, 100, 100);
                gradient.setColorAt(0, Qt::black);
                gradient.setColorAt(1, Qt::white);

                QBrush brush(gradient);
                slice->setBrush(brush);
            }
        }
        qreal hPos = (pieSize / 2) + (i / (qreal) dataTable.count());
        series->setPieSize(pieSize);
        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.5);
        chart->addSeries(series);
    }

    chartView.setChart(chart);
}
