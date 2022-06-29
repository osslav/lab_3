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

//конструктор
MyChart::MyChart()
{
    printer = IOCContainer::instance().Get_Object<IPrinterChart>();        //получаем принтер из контейнера
}

//функция обновления графика
void MyChart::updateGraphic(bool notColoredChart, int startPosition, int length)
{
    printer = IOCContainer::instance().Get_Object<IPrinterChart>();        //получаем из контейнера принтер
    notColored = notColoredChart;                                           //обновляем поля класса, касающиеся свойств представления графика
    currentLenght = length;
    currentStartPosition = startPosition;

    update();                       //обновляем представление графика
}

//обновление данных графика
bool MyChart::updateData(const QString& filePath)
{
    return IOCContainer::instance().Get_Object<IReadData>()->readData(m_dataList, filePath);   //получаем из контейнера ридер и сразу обновляем с его помощью данные
}

//функция обновления представления графика
void MyChart::update()
{
    printer->createChart(chartView, m_dataList, notColored, currentStartPosition, currentLenght);   //с помощью текущего принтера обновляем представление графика
}



