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
#include <QPdfWriter>

#include "themewidget.h"
#include "ioccontainer.h"

//конструктор виджета графика
ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent)
{
    typeComboBox = createTypeBox();                                 //определяем кнопку с всплывающим списком для выбора типа графика с помощью соответсвующей функции
    notColoredCheckBox = new QCheckBox("Черно-белый график");       //определяем остальные виджеты настройки графика с соответсвующими надписями
    printButton = new QPushButton("Печать графика");
    labelChooseTypeChart = new QLabel("Выберите тип диаграммы:");

    labelSizeSlider = new QLabel("Размер текущего фрагмента:");     //определяем виджеты, относящиеся к изменению количества элементов на графике
    labelValueSizeSlider = new QLabel();
    sizeSlider = new QSlider(Qt::Horizontal);

    labelPosSlider = new QLabel("Стартовая позиция текущего фрагмента:");   //определяем виджеты, относящиеся к изменению стартовой позиции графика
    labelValuePosSlider = new QLabel();
    posSlider = new QSlider(Qt::Horizontal);

    connectSignals();                                               //соединяем слоты-сигналы соответвующей функцией

    baseLayout = new QVBoxLayout();                                 //определяем основной лэйаут
    settingsLayout = new QHBoxLayout();                             //определяем лэйаут с настройками
    settingsLayout->addWidget(labelChooseTypeChart);                //помещаем на лэйаут с настройками соответствующие виджеты
    settingsLayout->addWidget(typeComboBox);
    settingsLayout->addWidget(notColoredCheckBox);
    settingsLayout->addWidget(printButton);
    settingsLayout->addStretch();                                   //добавляем на лэйаут с настройками разделитель
    baseLayout->addLayout(settingsLayout);                          //добавляем на основной лэйаут лэйаут с настройками(он буде самым верхним)


    addPrinterInContainer(TypeChart::bar);                          //по умолчанию график создается в виде столбчатой диаграмме, поэтому помещаем соответсвующий принтер с контейнер с помощью функции

    chart = new MyChart();                                          //создаем объект графика

    baseLayout->addWidget(chart->getChartView());                    //помещаем на основной лэйаут виджет с гарфиком, который получем от объекта класса MyChat

    sizeSliderLayout = new QHBoxLayout();                           //создаем лэйаут для настройки размера элементов на графике
    sizeSliderLayout->addWidget(labelSizeSlider);                   //помещаем на него соответствующие виджеты
    sizeSliderLayout->addWidget(sizeSlider);
    sizeSliderLayout->addWidget(labelValueSizeSlider);
    baseLayout->addLayout(sizeSliderLayout);                        //добавляем этот лэйаут на основной лэйаут

    posSliderLayout = new QHBoxLayout();                            //создаем лэйаут для настройки стартовой позиции графика
    posSliderLayout->addWidget(labelPosSlider);                     //помещаем на него соответствующие виджеты
    posSliderLayout->addWidget(posSlider);
    posSliderLayout->addWidget(labelValuePosSlider);
    baseLayout->addLayout(posSliderLayout);                         //добавляем этот лэйаут на основной лэйаут

    setLayout(baseLayout);                                          //устанавливаем основной лэйаут на виджет графика

    notColoredCheckBox->setChecked(true);                           //по умолчавнию график черно-белый

    updateUI();                                                     //обновляем графическую часть графика
}

//метод обновления данных на графике - принимает путь к файлу с данными в виде строки
void ThemeWidget::updateDataGraphic(const QString& filePath)
{
    QString exitMessage = chart->updateData(filePath);          //обновляем данные в графике и записываем сообшение о ходе выполнения функции
    if (exitMessage == "")
    {
        sizeSlider->setMinimum(0);      //если успешно, то обновляем крайние позиции ползунка количества элементов графика
        sizeSlider->setMaximum((chart->getCountElemInDataTable() > MAX_LENGHT_CHART) ? MAX_LENGHT_CHART : chart->getCountElemInDataTable());
        updateUI();                     //и обновляем графическую часть графика
    }
    else
    {
        QMessageBox messageBox;         //иначе - выводим сообщение об ошибке
        messageBox.critical(0,"Error", exitMessage);
        messageBox.setFixedSize(500,200);
    }
}

//метод соединения сигналов-слотов в виджете
void ThemeWidget::connectSignals()
{
    connect(typeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ThemeWidget::updateUI);                                                      //соединяем сигнал изменения выбранного типа графика с методом обновления виджета
    connect(notColoredCheckBox, &QCheckBox::toggled, this, &ThemeWidget::updateUI);             //соединяем сигнал изменения цвета(цветной или чб) графика с методом обновления виджета
    connect(printButton, SIGNAL(clicked()), this, SLOT(openFileDialogWindow()));                //соединяем сигнал нажатия на кнопку печати с методом вызова диалогового окна

    connect(sizeSlider, &QSlider::valueChanged, this, &ThemeWidget::updateUI);                  //соединяем изменения позиции слайдера размера графика с методом обновления виджета
    connect(posSlider, &QSlider::valueChanged, this, &ThemeWidget::updateUI);                   //соединяем изменения позиции слайдера стартовой позиции графика с методом обновления виджета
}

//метод открытия файлового диалогового окна и сохранения пдф по выбранному в нем пути
void ThemeWidget::openFileDialogWindow()
{
    QFileDialog fileDialog(this);                       //создаем файловое диалоговое окно

    fileDialog.setWindowTitle(tr ("Сохранить как"));    //устанавливаем поясняющую надпись для действия в нем

    fileDialog.setDirectory(".");                       //устанавливаем стартовую директорию

    fileDialog.setNameFilter(tr("PDF(*.pdf)"));         //установливаем фильтр файлов

    fileDialog.setViewMode(QFileDialog::Detail);        //устанавливаем режим просмотра

    QString fileName;                                   //строка для пути к выбранному файлу
    if(fileDialog.exec())                               //если окно открылось успешно
    {
        fileName = fileDialog.selectedFiles().first();  //сохраняем первый выбранный путь
    }

    QPdfWriter writer(fileName + ".pdf");               //определяем объект для создания pdf файлов(с нужным путем для сохранения файла в аргументе)

    writer.setCreator("Someone");                       //указываем создателя документа

    writer.setPageSize(QPagedPaintDevice::A4);          //устанавливаем размер страницы

    QPainter painter(&writer);                          //создаем отрисовщик для создания pdf

    chart->getChartView()->render(&painter);             //рендерим график в pdf
    painter.end();                                      //закрываем отрисовщик
}



//функция создания кнопки с всплывающим списком для выбора типа графика
QComboBox *ThemeWidget::createTypeBox() const
{
    QComboBox *themeComboBox = new QComboBox();         //создаем объект кнопки с всплывающим списком
    themeComboBox->addItem("Bar", TypeChart::bar);      //помещаем на нее тип графика "bar"
    themeComboBox->addItem("Pie", TypeChart::pie);      //помещаем на нее тип графика "pie"
    return themeComboBox;                               //возвращаем адрес созданного виджета
}
//слот обновления графической части графика
void ThemeWidget::updateUI()
{
    QString sizeFragment;                               //конвертируем в строку численное значение количества элементов на графике
    sizeFragment.setNum(sizeSlider->value());
    labelValueSizeSlider->setText(sizeFragment);        //и выводим его на строку, которая должна отображать это значение

    posSlider->setMinimum(0);                           //обновляем крайние точки ползунка для стартовой позиции графика
    posSlider->setMaximum((chart->getCountElemInDataTable() - sizeSlider->value()) < 0 ? 0 : (chart->getCountElemInDataTable() - sizeSlider->value()));

    QString startPosFragment;                           //аналогично конвертируем в строку численное значение стартовой позиции графика
    startPosFragment.setNum(posSlider->value());
    labelValuePosSlider->setText(startPosFragment);     //и выводим его на строку, которая должна отображать это значение

    TypeChart typeChart = static_cast<TypeChart>(       //получаем тип графика из кнопки с всплывающим списком типов графика
                typeComboBox->itemData(typeComboBox->currentIndex()).toInt());

    addPrinterInContainer(typeChart);                   //добавляем необходимый принтер в контейнер
    chart->updateGraphic(notColoredCheckBox->isChecked(), posSlider->value(), sizeSlider->value());  //обновляем график по полученным из виджетов настройкам
}

//функция добавления принтера в контейнер
void ThemeWidget::addPrinterInContainer(TypeChart type)
{
    switch (type)           //в зависимости от типа графика помещаем в контейнер соответвующий объект принтера
    {                       //с помощью статической функции instance() получаем единый для всей программы объект контейнера
    case TypeChart::bar :
        IOCContainer::instance().RegisterFactory<IPrinterChart, PrinterChartBar>();
        break;
    case TypeChart::pie :
        IOCContainer::instance().RegisterFactory<IPrinterChart, PrinterChartPie>();
        break;
    }
};

//функция обработчика события изменения размера окна
void ThemeWidget::resizeEvent(QResizeEvent *event)
{
    updateUI();     //ее задача - обновить график
}

