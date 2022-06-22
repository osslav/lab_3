#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>
#include <QBoxLayout>

#include "themewidget.h"

class MyTable;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private slots:
    void selectInTableSlot();

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
private:
    MyTable *tableWidget;
    ThemeWidget *themeWidget;
};

class MyTable : public QWidget
{
    Q_OBJECT

private slots:
    void changeDirectory();
    void selectInTableSlot(const QItemSelection &selected, const QItemSelection &deselected);

private:
    QVBoxLayout *tableLayout;
    QPushButton *changeDirectoryButton;
    QFileSystemModel *fileModel;
    QTableView *tableView;

    QString currentPath;

    bool addReaderInContainer(QString fileExtension);

signals:
    void directoryChanged();

public:
    MyTable(QWidget *parent = 0);

    QString getDirectory() { return currentPath; };

    ~MyTable() {};
};

#endif // MAINWINDOW_H
