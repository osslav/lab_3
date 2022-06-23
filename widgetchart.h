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

class WidgetChart : public QWidget
{
    Q_OBJECT
public:
    WidgetChart(QWidget* parent = nullptr);
    virtual ~WidgetChart() {}//{ if (chartView) delete chartView; }

    void updateWidget(bool notColoredChart, int length);

    bool updateDataGraphic(const QString& filePath);

    QChartView* getChartView() { return &chartView; };

    int getCountElemInDataTable() { return (m_dataTable.isEmpty() ? 0 : m_dataTable[0].size()); }

private:
    bool notColored = false;
    DataTable m_dataTable;
    QChartView chartView;
    std::shared_ptr<IPrinterChart> printer;
    int currentLenght;

    void updateGraphic();
};

#endif /* WIDGETCHART_H */
