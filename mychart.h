#ifndef MYCHART_H
#define MYCHART_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QPushButton>

#include "ioccontainer.h"
#include "printchart.h"
#include "readdata.h"


QT_CHARTS_USE_NAMESPACE

//класс графика(который не является виджетом, а хранит в себе данные и представление графика)
class MyChart
{
public:
    MyChart();      //конструктор

    void updateGraphic(bool notColoredChart, int startPosition, int length);        //функция обновления графика

    QString updateData(const QString& filePath);                                       //фукнция обновления данных графика

    QChartView* getChartView() { return &chartView; };                              //функция получения представления графика

    int getCountElemInDataTable() { return m_dataList.size(); }                     //функция получения количества данных, даступных для построения по ним графика

private:
    bool notColored = false;                    //флаг - цветной ли график
    DataList m_dataList;                        //таблица с данными
    QChartView chartView;                       //представление графика
    std::shared_ptr<IPrinterChart> printer;     //текущий принтер
    int currentLenght;                          //текущее количество элементов на графике
    int currentStartPosition;                   //текущая стартовая позция графика

    void update();                              //функция обновления представления графика
};

#endif /* MYCHART_H */
