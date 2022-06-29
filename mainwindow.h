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

//класс главного окна
class MainWindow : public QMainWindow
{
	Q_OBJECT

private slots:
    void selectInTableSlot();               //слот обрабатывающий нажатие на элемент файловой таблицы

public:
    MainWindow(QWidget *parent = 0);        //конструктор
private:
    MyTable *tableWidget;                   //содержит виджеты с файловой таблицей
    ThemeWidget *themeWidget;               //и виджет с графиком
};

//класс виджет файловой таблицы с возможностью изменить текущую папку
class MyTable : public QWidget
{
    Q_OBJECT

private slots:
    void changeDirectory();                 //слот изменения директории таблицы
    void selectInTableSlot(const QItemSelection &selected, const QItemSelection &deselected);       //слот обрабатывающий нажатие на элемент файловой таблицы

private:
    QVBoxLayout *tableLayout;               //основной лэйаут
    QPushButton *changeDirectoryButton;     //кнопка изменения директории таблицы
    QFileSystemModel *fileModel;            //используемая файловая модель
    QTableView *tableView;                  //файловая таблица

    QString currentPath;                    //путь к выбранному в таблице файлу

    bool addReaderInContainer(QString fileExtension);   //функция добавления ридера в ioc контейнер

signals:
    void directoryChanged();                            //сигнал выбора нового файла

public:
    MyTable(QWidget *parent = 0);                       //конструктор

    QString getDirectory() { return currentPath; };     //функция получения пути к выбранному файлу
};

#endif // MAINWINDOW_H
