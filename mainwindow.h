#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QBoxLayout>

#include "themewidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
private slots:
    void changeDirectory();
    void selectInTableSlot(const QItemSelection &selected, const QItemSelection &deselected);

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private:
	QFileSystemModel *fileModel;

	QTableView *tableView;
    ThemeWidget *themeWidget;
    QVBoxLayout *tableLayout;
    QPushButton *changeDirectoryButton;

    bool addReaderInContainer(QString fileExtension);
};

#endif // MAINWINDOW_H
