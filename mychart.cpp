#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>
#include <QFileDialog>
#include <QtSql>
#include <QSqlTableModel>

#include "widgetchart.h"
#include "ioccontainer.h"

MyChart::MyChart()
{
    printer = IOCContainer::instance().GetttObject<IPrinterChart>();
}


void MyChart::updateGraphic(bool notColoredChart, int length)
{
    printer = IOCContainer::instance().GetttObject<IPrinterChart>();
    notColored = notColoredChart;
    currentLenght = length;

    update();
}

bool MyChart::updateData(const QString& filePath)
{
    return IOCContainer::instance().GetttObject<IReadData>()->readData(m_dataTable, filePath);
}

void MyChart::update()
{
    printer->createChart(chartView, m_dataTable, notColored, currentLenght);
}



