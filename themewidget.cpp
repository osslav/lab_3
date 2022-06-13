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

#include "themewidget.h"


ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent)
{
    typeComboBox = createTypeBox();
    notColoredCheckBox = new QCheckBox("Черно-белый график");
    printButton = new QPushButton("Печать графика");
    labelChooseTypeChart = new QLabel("Выберите тип диаграммы:");

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

    chart = new WidgetChart(this);

    baseLayout->addWidget(chart->getChartView());

    setLayout(baseLayout);

    // Set defaults
    notColoredCheckBox->setChecked(true);
    updateUI();
}

ThemeWidget::~ThemeWidget()
{
    delete baseLayout;
    delete settingsLayout;
    delete labelChooseTypeChart;
    delete typeComboBox;
    delete notColoredCheckBox;
    delete printButton;
    delete chart;
}

void ThemeWidget::connectSignals()
{
    connect(typeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &ThemeWidget::updateUI);
    connect(notColoredCheckBox, &QCheckBox::toggled, this, &ThemeWidget::updateUI);
    connect(printButton, SIGNAL(clicked()), this, SLOT(openFileDialogWindow()));
}

void ThemeWidget::openFileDialogWindow()
{
    QFileDialog *fileDialog = new QFileDialog(this);
             // определить заголовок файла
    fileDialog-> setWindowTitle (tr ("Открыть файл"));
             // Установить путь к файлу по умолчанию
    fileDialog->setDirectory(".");
             // Установить фильтр файлов
    fileDialog->setNameFilter(tr("BD(*.sqlite *.json)"));
             // Настройка позволяет выбрать несколько файлов, по умолчанию используется только один файл QFileDialog :: ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
             // Установить режим просмотра
    fileDialog->setViewMode(QFileDialog::Detail);
             // выводим путь ко всем выбранным файлам
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }
}




QComboBox *ThemeWidget::createTypeBox() const
{
    // type layout
    QComboBox *themeComboBox = new QComboBox();
    themeComboBox->addItem("Area", TypeWidgetChart::Area);
    themeComboBox->addItem("Bar", TypeWidgetChart::Bar);
    themeComboBox->addItem("Line", TypeWidgetChart::Line);
    themeComboBox->addItem("Pie", TypeWidgetChart::Pie);
    themeComboBox->addItem("Spline", TypeWidgetChart::Spline);
    themeComboBox->addItem("Scatter", TypeWidgetChart::Scatter);
    return themeComboBox;
}

void ThemeWidget::updateUI()
{
    TypeWidgetChart typeChart = static_cast<TypeWidgetChart>(
                typeComboBox->itemData(typeComboBox->currentIndex()).toInt());

    chart->updateWidget(typeChart, notColoredCheckBox->isChecked());
}

