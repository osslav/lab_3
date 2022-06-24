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


ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent)
{
    typeComboBox = createTypeBox();
    notColoredCheckBox = new QCheckBox("Черно-белый график");
    printButton = new QPushButton("Печать графика");
    labelChooseTypeChart = new QLabel("Выберите тип диаграммы:");

    labelSizeSlider = new QLabel("Размер текущего фрагмента:");
    labelValueSizeSlider = new QLabel();
    sizeSlider = new QSlider(Qt::Horizontal);

    labelPosSlider = new QLabel("Стартовая позиция текущего фрагмента:");
    labelValuePosSlider = new QLabel();
    posSlider = new QSlider(Qt::Horizontal);

    connectSignals();
    // create layout
    baseLayout = new QVBoxLayout();
    settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(labelChooseTypeChart);
    settingsLayout->addWidget(typeComboBox);
    settingsLayout->addWidget(notColoredCheckBox);
    settingsLayout->addWidget(printButton);
    settingsLayout->addStretch();
    baseLayout->addLayout(settingsLayout);

    //create chart
    addPrinterInContainer(TypeChart::bar);
    chart = new MyChart();

    baseLayout->addWidget(chart->getChartView());

    sizeSliderLayout = new QHBoxLayout();
    sizeSliderLayout->addWidget(labelSizeSlider);
    sizeSliderLayout->addWidget(sizeSlider);
    sizeSliderLayout->addWidget(labelValueSizeSlider);
    baseLayout->addLayout(sizeSliderLayout);

    posSliderLayout = new QHBoxLayout();
    posSliderLayout->addWidget(labelPosSlider);
    posSliderLayout->addWidget(posSlider);
    posSliderLayout->addWidget(labelValuePosSlider);
    baseLayout->addLayout(posSliderLayout);

    setLayout(baseLayout);

    // Set defaults
    notColoredCheckBox->setChecked(true);

    updateUI();
}

ThemeWidget::~ThemeWidget()
{
    delete labelChooseTypeChart;
    delete typeComboBox;
    delete notColoredCheckBox;
    delete printButton;
    delete chart;
    delete settingsLayout;
    delete baseLayout;
}

void ThemeWidget::updateDataGraphic(const QString& filePath)
{
    if (chart->updateData(filePath))
    {
        sizeSlider->setMinimum(0);
        sizeSlider->setMaximum((chart->getCountElemInDataTable() > MAX_LENGHT_CHART) ? MAX_LENGHT_CHART : chart->getCountElemInDataTable());
        updateUI();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Unable to plot file: " + filePath);
        messageBox.setFixedSize(500,200);
    }
}

void ThemeWidget::connectSignals()
{
    connect(typeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ThemeWidget::updateUI);
    connect(notColoredCheckBox, &QCheckBox::toggled, this, &ThemeWidget::updateUI);
    connect(printButton, SIGNAL(clicked()), this, SLOT(openFileDialogWindow()));

    connect(sizeSlider, &QSlider::valueChanged, this, &ThemeWidget::updateUI);
    connect(posSlider, &QSlider::valueChanged, this, &ThemeWidget::updateUI);
}

void ThemeWidget::openFileDialogWindow()
{
    QFileDialog fileDialog(this);
             // определить заголовок файла
    fileDialog.setWindowTitle (tr ("Сохранить как"));
             // Установить путь к файлу по умолчанию
    fileDialog.setDirectory(".");
             // Установить фильтр файлов
    fileDialog.setNameFilter(tr("PDF(*.pdf)"));
             // Настройка позволяет выбрать несколько файлов, по умолчанию используется только один файл QFileDialog :: ExistingFiles
    //fileDialog.setFileMode(QFileDialog::ExistingFiles);
             // Установить режим просмотра
    fileDialog.setViewMode(QFileDialog::Detail);
             // выводим путь ко всем выбранным файлам
    QString fileName;
    if(fileDialog.exec())
    {
        fileName = fileDialog.selectedFiles().first();
    }

    QPdfWriter writer(fileName + ".pdf");

    writer.setCreator("Someone");//Указываем создателя документа

    writer.setPageSize(QPagedPaintDevice::A4);//Устанавливаем размер страницы

    QPainter painter(&writer);

    chart->getChartView()->render(&painter);
    painter.end();
}




QComboBox *ThemeWidget::createTypeBox() const
{
    // type layout
    QComboBox *themeComboBox = new QComboBox();
    themeComboBox->addItem("Bar", TypeChart::bar);
    themeComboBox->addItem("Pie", TypeChart::pie);
    return themeComboBox;
}

void ThemeWidget::updateUI()
{
    QString sizeFragment;
    sizeFragment.setNum(sizeSlider->value());
    labelValueSizeSlider->setText(sizeFragment);

    posSlider->setMinimum(0);
    posSlider->setMaximum((chart->getCountElemInDataTable() - sizeSlider->value()) < 0 ? 0 : (chart->getCountElemInDataTable() - sizeSlider->value()));

    QString startPosFragment;
    startPosFragment.setNum(posSlider->value());
    labelValuePosSlider->setText(startPosFragment);

    TypeChart typeChart = static_cast<TypeChart>(
                typeComboBox->itemData(typeComboBox->currentIndex()).toInt());

    addPrinterInContainer(typeChart);
    chart->updateGraphic(notColoredCheckBox->isChecked(), posSlider->value(), sizeSlider->value());
}

void ThemeWidget::addPrinterInContainer(TypeChart type)
{
    switch (type)
    {
    case TypeChart::bar :
        IOCContainer::instance().RegisterFactory<IPrinterChart, PrinterChartBar>();
        break;
    case TypeChart::pie :
        IOCContainer::instance().RegisterFactory<IPrinterChart, PrinterChartPie>();
        break;
    }
};

void ThemeWidget::resizeEvent(QResizeEvent *event)
{
    //QWidget::resizeEvent(QResizeEvent *event)

    updateUI();
}

