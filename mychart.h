#ifndef WIDGETCHART_H
#define WIDGETCHART_H

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

    int getCountElemInDataTable() { return (m_dataTable.isEmpty() ? 0 : m_dataTable[0].size()); }

private:
    bool notColored = false;
    DataTable m_dataTable;
    QChartView chartView;
    std::shared_ptr<IPrinterChart> printer;
    int currentLenght;

    void update();
};

#endif /* WIDGETCHART_H */
