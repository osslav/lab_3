#ifndef PRINTCHART_H
#define PRINTCHART_H

#include <QtCharts>

#include "ioccontainer.h"

//используемые типы данных для данных графика
using Data = QPair<QPointF, QString>;   //элемент графика - состоит из двух чисел и строки
using DataList = QList<Data>;           //список элементов графика

//перечиследние возможных типов графиков для построения
enum TypeChart
{
    pie,            //круговая диаграмма
    bar,            //столбчатая диаграмма
};

//класс-интерфейс, который заносит данные из DataList в представление QChartView
class IPrinterChart
{
public:
   //чистая виртуальная функция которая должна заносить данные из DataList в представление QChartView
    virtual void createChart(QChartView &chartView, const DataList& dataList, bool notColored = false, int startPosition = 0, int length = 0) = 0;
};

//реализация класса IPrinterChart,который заносит данные из DataList в представление QChartView в формате Bar
class PrinterChartBar : public IPrinterChart
{
public:
   //виртуальная функция которая заносит данные из DataList в представление QChartView в формате bar в соответствии с переданными аргументами
    virtual void createChart(QChartView &chartView, const DataList& data, bool notColored = false, int startPosition = 0, int length = 0);
};

//реализация класса IPrinterChart,который заносит данные из DataList в представление QChartView в формате Pie
class PrinterChartPie : public IPrinterChart
{
public:
    //виртуальная функция которая заносит данные из DataList в представление QChartView в формате pie в соответствии с переданными аргументами
     virtual void createChart(QChartView &chartView, const DataList& dataList, bool notColored = false, int startPosition = 0, int length = 0);
};


#endif // PRINTCHART_H
