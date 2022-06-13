#ifndef WIDGETCHART_H
#define WIDGETCHART_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QPushButton>


QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;
using DataTable = QList<DataList>;

QT_CHARTS_USE_NAMESPACE

enum TypeWidgetChart
{
    Area,
    Bar,
    Pie,
    Line,
    Spline,
    Scatter,
};

class WidgetChart : public QWidget
{
    Q_OBJECT
public:
    WidgetChart(QWidget* parent = nullptr);
    virtual ~WidgetChart() {}//{ if (chartView) delete chartView; }

    void updateWidget(TypeWidgetChart newType, bool notColoredChart);

    QChartView* getChartView() { return chartView; };

private:
    bool notColored = false;
    TypeWidgetChart typeWidget = TypeWidgetChart::Area;
    DataTable m_dataTable = generateRandomData(1, 10, 10);
    QChartView *chartView = nullptr;

    static DataTable generateRandomData(int listCount, int valueMax, int valueCount);

    void createAreaChart() const;
    void createBarChart() const;
    void createPieChart() const;
    void createLineChart() const;
    void createSplineChart() const;
    void createScatterChart() const;

};

#endif /* THEMEWIDGET_H */
