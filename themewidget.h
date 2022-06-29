#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QPushButton>

#include "mychart.h"


QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE

//максимальное количество точек в графике
const int MAX_LENGHT_CHART = 20;

//виджет отображения и печати данных
class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = nullptr);    //конструтор виджета

    ~ThemeWidget() { delete chart; }                    //деструктор

    void updateDataGraphic(const QString& filePath);    //функция обновления данных графика

private Q_SLOTS:
    void updateUI();                                    //слот обновления графической части графика
    void openFileDialogWindow();                        //слот открытия диалогового окна(для печати в pdf)

private:
    QComboBox *createTypeBox() const;                   //функция создания кнопки с всплывающим списком для выбора типа графика
    void connectSignals();                              //функция для соединения сигналов-слотов этого виджета

    void addPrinterInContainer(TypeChart type);         //функция добавления принтера в контейнер

    void virtual resizeEvent(QResizeEvent *event);      //функция обработчика события изменения размера окна

    QVBoxLayout *baseLayout;                            //основной лэйаут
    QHBoxLayout *settingsLayout;                        //лэйаут настроек графика

    QLabel *labelChooseTypeChart;                       //строка пояснения к настройкам графика
    QComboBox *typeComboBox;                            //кнопка с всплывающим списком для выбора типа графика
    QCheckBox *notColoredCheckBox;                      //флажок с текстовой меткой - выбор будет ли график черно-белым
    QPushButton *printButton;                           //кнопка печати в пдф

    MyChart *chart;                                      //объект графика MyChart

    QHBoxLayout *sizeSliderLayout;                      //лэйаут с ползунком для изменеия количества элементов на графике
    QLabel *labelSizeSlider;                            //строка пояснения к ползунку
    QSlider *sizeSlider;                                //сам ползунок для изменеия количества элементов на графике
    QLabel *labelValueSizeSlider;                       //строка отображающая значение ползунка в данный момент

    QHBoxLayout *posSliderLayout;                       //лэйаут с ползунком для изменеия стартовой позиции графика
    QLabel *labelPosSlider;                             //строка пояснения к ползунку
    QSlider *posSlider;                                 //сам ползунок для изменеия стартовой позиции графика
    QLabel *labelValuePosSlider;                        //строка отображающая значение ползунка в данный момент
};

#endif /* THEMEWIDGET_H */
