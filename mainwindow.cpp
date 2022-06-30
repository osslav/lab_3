#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>
#include <QDebug>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QTime>
#include <QtCharts/QBarCategoryAxis>

//определяем статическое поле ioc контейнера
int IOCContainer::s_typeId = 121;

//конструктор главного окна
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    themeWidget = new ThemeWidget();        //создаем 2 виджета для окна - файловая таблица и виджет графика
    tableWidget = new MyTable();

    this->setGeometry(100, 100, 1500, 500);             //устанавливаем размер главного окна
    this->setStatusBar(new QStatusBar(this));           //устанавливаем строку статуса окна(которое предлагает выбрать путь к файлу)
    this->statusBar()->showMessage("Выберите: ");


    QSplitter *splitter = new QSplitter(parent);        //создаем разделитель

    splitter->addWidget(tableWidget);                   //и помещаем на него виджеты
    splitter->addWidget(themeWidget);

    setCentralWidget(splitter);                         //устанавливаем разделаитель главным виджетом окна

    connect(tableWidget, SIGNAL(directoryChanged()), this, SLOT(selectInTableSlot()));  //соединение сигнала выбора нового файла с соответствующим слотом главного окна
}

//слот обрабатывающий нажатие на элемент файловой таблицы
void MainWindow::selectInTableSlot()
{
    QString filePath = tableWidget->getDirectory();                     //получаем путь к выбранному файлу

    this->statusBar()->showMessage("Выбранный файл : " + filePath);     // Размещаем инфо в statusbar относительно выбранного файла

    themeWidget->updateDataGraphic(filePath);                           //передаем в виджет графика этот путь
}



//конструктор виджета с файловой таблицей
MyTable::MyTable(QWidget *parent)
    : QWidget(parent)
{
    QString homePath = QDir::homePath();                                //записываем путь по умолчанию для данной системы

    fileModel = new QFileSystemModel(this);                             //определяем файловую модель
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);           //устанавливаем фильтры для нее : рассматриваем файлы, не являющиеся специальными

    fileModel->setRootPath(homePath);                                   //устанавливаем файловую систему на стартовую позицию

    tableLayout = new QVBoxLayout(this);                                //определяем лэйаут, таблицу и кнопку измененеия директории
    tableView = new QTableView;
    changeDirectoryButton = new QPushButton("Change directory");
    tableLayout->addWidget(tableView);                                  //помещаем таблицу и кнопку на лэйаут
    tableLayout->addWidget(changeDirectoryButton);
    tableView->setModel(fileModel);                                     //привязываем к таблице файловую модель

    QItemSelectionModel *selectionModelTable = tableView->selectionModel(); //получаем адрес объекта, который отслеживает выбранные элементы в таблице

    connect(changeDirectoryButton, SIGNAL(clicked()), this, SLOT(changeDirectory()));   //соединяем сигнал нажатия на кнопку со слотом изменения директории

    connect(selectionModelTable, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),  //соединяем сигнал изменения выбранного файла с соответствующим слотом
            this, SLOT(selectInTableSlot(const QItemSelection &, const QItemSelection &)));
}

//слот изменеия директории таблицы
void MyTable::changeDirectory()
{
    QFileDialog fileDialog(this);                       //создаем файловое диалоговое окно

    fileDialog.setFileMode(QFileDialog::Directory);     //запускаем его в режиме выбора каталога

    fileDialog.setDirectory(QDir::homePath());          //устанавливаем путь по умолчанию

    QString filePath;                                   //строка в которую запишем полученный путь
    if(fileDialog.exec())                               //запускаем диалоговое окно и если оно открылось продолжаем
    {
        filePath = fileDialog.selectedFiles().first();  //записываем полученный от диалогового окна путь

        tableView->setRootIndex(fileModel->setRootPath(filePath));  //устанавливаем этот путь для файловой таблицы
    }
}


//слот обрабатывающий нажатие на элемент файловой таблицы
void MyTable::selectInTableSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    QModelIndexList indexs =  selected.indexes();       //сохраняем индекс выбранного элемента таблицы
    QString filePath = "";                              //строка в которую запишем путь к выбранному файлу

    if (indexs.count() >= 1)                            //если выбрано больше 0 элементов
    {
        QModelIndex ix =  indexs.constFirst();          //сохраняем ндекс первого выбранного элемента таблицы
        filePath = fileModel->filePath(ix);             //сохраняем путь к файлу с этим индексом в таблице

        QString fileExtension = fileModel->filePath(indexs.constFirst());   //выделяем из пути к файлу разрешение этого файла
        fileExtension.remove(0, fileExtension.indexOf('.'));
        if (addReaderInContainer(fileExtension))                            //записываем в контейнер ридер по полученному разрешению
        {
            currentPath = fileModel->filePath(indexs.constFirst());         //если запись прошла успешно - сохраняем этот путь в поле класса
            emit directoryChanged();                                        //и вызываем сигнал изменения директории выдленного файла
        }
        else
        {
            QMessageBox messageBox;                                         //если запись прошла неудачно выводим окно ошибки
            messageBox.critical(0,"Error","Не поддерживается тип файла " + filePath);
            messageBox.setFixedSize(500,200);
        }
    }
}

//функция добавления ридера в ioc контейнер по соответствующему разрешению файла
bool MyTable::addReaderInContainer(QString fileExtension)
{
    if (fileExtension == ".sqlite")             //если файл это бд sqlite то помещаем в контейнер нужный ридер и возвращаем флаг успешного завершения
     {                                          //с помощью статической функции instance() получаем единый для всей программы объект контейнера
        IOCContainer::instance().RegisterFactory<IReadData, ReadDataSqlite>();
        return true;
    }

    if (fileExtension == ".json")               //если файл это json то помещаем в контейнер нужный ридер и возвращаем флаг успешного завершения
    {
        IOCContainer::instance().RegisterFactory<IReadData, ReadDataJson>();
        return true;
    }


    //если не удалось найти переданное разрешение файла, то возвращаем флаг ошибки
    return false;
}
