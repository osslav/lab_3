#include "printchart.h"

//функция которая заносит данные из DataList в представление QChartView в формате bar в соответствии с переданными аргументами
void PrinterChartBar::createChart(QChartView &chartView, const DataList& dataList, bool notColored, int startPosition, int length)
{
        QChart *chart = new QChart();                   //создаем новый график
        chart->setTitle("Bar chart");                   //устанавливаем заголовок

        QBarSeries *series = new QBarSeries(chart);     //создаем объект, представляющий серию данных в виде вертикальных полос

        int endPosition = length + startPosition;       //вычисляем индекс последнего элемента графика
        if (dataList.count() < endPosition)
            endPosition = dataList.count();

        for (int i(startPosition); i < endPosition; i++)    //цикл по индексам элементов, которые должны быть отобржены на графике
        {
            QBarSet *set = new QBarSet(dataList[i].second); //создаем новый набор элементов для графика
            *set << dataList[i].first.y();                      //помещаем на него значение текущего элемента

            if (notColored)                                 //если график должен быть черно-белый
            {
                QLinearGradient gradient(0, 0, chartView.size().width(), chartView.size().height());    //создаем градиент от черного к белому
                gradient.setColorAt(0, Qt::black);
                gradient.setColorAt(1, Qt::white);

                QBrush brush(gradient);                     //создаем кисть с этим градиентом
                set->setBrush(brush);                       //применяем эту кисть
            }
            series->append(set);                            //заносим текущий набор элементов в серию
        }
        chart->addSeries(series);                           //записываем серию на график
        chart->createDefaultAxes();                         //создаем оси по умолчанию

        chartView.setChart(chart);                          //помещаем график на представление
}

//функция которая заносит данные из DataList в представление QChartView в формате pie в соответствии с переданными аргументами
void PrinterChartPie::createChart(QChartView &chartView, const DataList& dataList, bool notColored, int startPosition, int length)
{
    QChart *chart = new QChart();           //создаем новый график
    chart->setTitle("Pie chart");           //устанавливаем заголовок

    QPieSeries *series = new QPieSeries(chart); //создаем объект, представляющий серию данных в виде круговой диаграммы
    int j = 0;                              //индекс текущего элемента в списке данных
    int lightness = 0;                      //яркость текущего элемента(нужен для черно-белого варианта)

    int endPosition = length + startPosition;   //вычисляем индекс последнего элемента графика
    if (dataList.count() < endPosition)
        endPosition = dataList.count();

    for (const Data &data : dataList)           //цикл по элементам переданного списка данных
    {
        if (j >= endPosition)                   //выходим из цикла если вышли за пределы рассматриваемого диапазона элементов на графике
            break;
        if (j >= startPosition)                 //если индекс больше стартовой позиции графика, то заносим текущий элемент на график
        {
            QPieSlice *slice = series->append(data.second, data.first.y());     //добавляем элемент на график

            if (notColored)                     //если черно-белый
            {
                lightness += 255 / length;      //считаем яркость текущего элемента
                QColor color(0,0,0,lightness);  //получаем его цвет
                QBrush brush(color);            //устанавливаем этот цвет на кисть
                slice->setBrush(brush);         //полученной кистью раскрашиваем элемент графика
            }
        }
        j++;
    }

    chart->addSeries(series);               //добавляем полученную серию данных на график


    chartView.setChart(chart);              //помещаем график на представление
}
