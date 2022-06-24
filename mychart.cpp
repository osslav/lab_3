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

#include "mychart.h"
#include "ioccontainer.h"

MyChart::MyChart()
{
    printer = IOCContainer::instance().GetttObject<IPrinterChart>();
}


void MyChart::updateGraphic(bool notColoredChart, int startPosition, int length)
{
    printer = IOCContainer::instance().GetttObject<IPrinterChart>();
    notColored = notColoredChart;
    currentLenght = length;
    currentStartPosition = startPosition;

    update();
}

bool MyChart::updateData(const QString& filePath)
{
    return IOCContainer::instance().GetttObject<IReadData>()->readData(m_dataList, filePath);
}

void MyChart::update()
{
    printer->createChart(chartView, m_dataList, notColored, currentStartPosition, currentLenght);
}



