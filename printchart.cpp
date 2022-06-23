#include "printchart.h"

void PrinterChartBar::createChart(QChartView &chartView, const DataList& dataList, bool notColored, int length)
{

        QChart *chart = new QChart();
        chart->setTitle("Bar chart");

        QBarSeries *series = new QBarSeries(chart);

        if (dataList.count() < length)
            length = dataList.count();

        for (int i(0); i < length; i++)
        {
            QBarSet *set = new QBarSet(dataList[i].second);
            *set << dataList[i].first.y();

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

void PrinterChartPie::createChart(QChartView &chartView, const DataList& dataList, bool notColored, int length)
{
    QChart *chart = new QChart();
    chart->setTitle("Pie chart");

        QPieSeries *series = new QPieSeries(chart);
        int j = 0;
        int lightness = 0;
        if (dataList.count() < length)
            length = dataList.count();
        for (const Data &data : dataList)
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

        series->setVerticalPosition(0.5);
        chart->addSeries(series);


    chartView.setChart(chart);
}
