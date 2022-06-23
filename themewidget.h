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

const int MAX_LENGHT_CHART = 20;

class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = nullptr);
    ~ThemeWidget();

    void updateDataGraphic(const QString& filePath);

private Q_SLOTS:
    void updateUI();
    void openFileDialogWindow();

private:
    QComboBox *createTypeBox() const;
    void connectSignals();

    void addPrinterInContainer(TypeChart type);

    void virtual resizeEvent(QResizeEvent *event);

    QVBoxLayout *baseLayout;
    QHBoxLayout *settingsLayout;

    QLabel *labelChooseTypeChart;
    QComboBox *typeComboBox;
    QCheckBox *notColoredCheckBox;
    QPushButton *printButton;

    MyChart *chart;

    QHBoxLayout *sizeSliderLayout;
    QLabel *labelSizeSlider;
    QSlider *sizeSlider;
    QLabel *labelValueSlider;
};

#endif /* THEMEWIDGET_H */
