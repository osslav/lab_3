#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QPushButton>

#include "widgetchart.h"


QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
QT_END_NAMESPACE



class ThemeWidget: public QWidget
{
    Q_OBJECT
public:
    explicit ThemeWidget(QWidget *parent = nullptr);
    ~ThemeWidget();

private Q_SLOTS:
    void updateUI();
    void openFileDialogWindow();

private:
    QComboBox *createTypeBox() const;
    void connectSignals();

    QVBoxLayout *baseLayout;
    QHBoxLayout *settingsLayout;

    QLabel *labelChooseTypeChart;
    QComboBox *typeComboBox;
    QCheckBox *notColoredCheckBox;
    QPushButton *printButton;

    WidgetChart *chart;
};

#endif /* THEMEWIDGET_H */
