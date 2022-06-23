#include "printchart.h"

void PrinterChartBar::createChart(QChartView &chartView, const DataTable& dataTable, bool notColored, int length)
{
    if (!dataTable.isEmpty())
    {
        QChart *chart = new QChart();
        chart->setTitle("Bar chart");

        QBarSeries *series = new QBarSeries(chart);
        const DataList &data = dataTable.first();

        if (data.count() < length)
            length = data.count();

        for (int i(0); i < length; i++)
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

void PrinterChartPie::createChart(QChartView &chartView, const DataTable& dataTable, bool notColored, int length)
{
    QChart *chart = new QChart();
    chart->setTitle("Pie chart");

    qreal pieSize = 1.0 / dataTable.count();
    for (int i = 0; i < dataTable.count(); i++) {
        QPieSeries *series = new QPieSeries(chart);
        int j = 0;
        int lightness = 0;
        if (dataTable[i].count() < length)
            length = dataTable[i].count();
        for (const Data &data : dataTable[i])
        {
            if (j >= length)
                break;

            QPieSlice *slice = series->append(data.second, data.first.y());

            if (notColored)
            {
                lightness += 255 / length;
                QColor color(0,0,0,lightness);
                QBrush brush(color);
                slice->setBrush(brush);
            }
            j++;
        }
        qreal hPos = (pieSize / 2) + (i / (qreal) dataTable.count());
        series->setPieSize(pieSize);
        series->setHorizontalPosition(hPos);
        series->setVerticalPosition(0.5);
        chart->addSeries(series);
    }

    chartView.setChart(chart);
}
