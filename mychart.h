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

class MyChart
{
public:
    MyChart();

    void updateGraphic(bool notColoredChart, int length);

    bool updateData(const QString& filePath);

    QChartView* getChartView() { return &chartView; };

    int getCountElemInDataTable() { return m_dataList.size(); }

private:
    bool notColored = false;
    DataList m_dataList;
    QChartView chartView;
    std::shared_ptr<IPrinterChart> printer;
    int currentLenght;

    void update();
};

#endif /* MYCHART_H */
