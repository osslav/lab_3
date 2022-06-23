#include "printchart.h"

void PrinterChartBar::createChart(QChartView &chartView, const DataTable& dataTable, bool notColored)
{
    if (!dataTable.isEmpty())
    {
        QChart *chart = new QChart();
        chart->setTitle("Bar chart");

        QBarSeries *series = new QBarSeries(chart);
        const DataList &data = dataTable.first();

        for (int i(0); i < data.count(); i++)
        {
            QBarSet *set = new QBarSet(data[i].second);
            *set << data[i].first.y();

            if (notColored)
            {
                QLinearGradient gradient(0, 0, chartView.size().width(), chartView.size().height());
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
}

void PrinterChartPie::createChart(QChartView &chartView, const DataTable& dataTable, bool notColored)
{
    QChart *chart = new QChart();
    chart->setTitle("Pie chart");

    qreal pieSize = 1.0 / dataTable.count();
    for (int i = 0; i < dataTable.count(); i++) {
        QPieSeries *series = new QPieSeries(chart);
        int lightness = 0;
        for (const Data &data : dataTable[i]) {
            QPieSlice *slice = series->append(data.second, data.first.y());

            if (notColored)
            {
                QColor color(0,0,0,lightness);
                QBrush brush(color);
                slice->setBrush(brush);

                lightness += 255 / dataTable[i].count();
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
