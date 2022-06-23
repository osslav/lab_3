#ifndef PRINTCHART_H
#define PRINTCHART_H

#include <QtCharts>

#include "ioccontainer.h"

using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;
using DataTable = QList<DataList>;

enum TypeChart
{
    pie,
    bar,
};

class IPrinterChart
{
public:
    IPrinterChart() {};

    virtual void createChart(QChartView &chartView, const DataTable& data, bool notColored = false, int length = 0) = 0;
};


class PrinterChartBar : public IPrinterChart
{
public:
    PrinterChartBar() {};

    virtual void createChart(QChartView &chartView, const DataTable& data, bool notColored = false, int length = 0);
};

class PrinterChartPie : public IPrinterChart
{
public:
    PrinterChartPie() {};

     virtual void createChart(QChartView &chartView, const DataTable& data, bool notColored = false, int length = 0);
};


#endif // PRINTCHART_H
