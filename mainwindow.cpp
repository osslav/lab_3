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

int IOCContainer::s_typeId = 121;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    themeWidget = new ThemeWidget();
    //Устанавливаем размер главного окна
    this->setGeometry(100, 100, 1500, 500);
	this->setStatusBar(new QStatusBar(this));
	this->statusBar()->showMessage("Choosen Path: ");
	QString homePath = QDir::homePath();
	// Определим  файловой системы:
//	dirModel =  new QFileSystemModel(this);
//	dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
//	dirModel->setRootPath(homePath);

	fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

	fileModel->setRootPath(homePath);
	//Показать как дерево, пользуясь готовым видом:

//	treeView = new QTreeView();
//	treeView->setModel(dirModel);

//	treeView->expandAll();

	QSplitter *splitter = new QSplitter(parent);

    tableLayout = new QVBoxLayout();
	tableView = new QTableView;
    changeDirectoryButton = new QPushButton("Change directory");
    tableLayout->addWidget(tableView);
    tableLayout->addWidget(changeDirectoryButton);
	tableView->setModel(fileModel);
//    splitter->addWidget(treeView);
    splitter->addWidget(tableView);
    splitter->addWidget(changeDirectoryButton);

//1.Добавление диаграммы
//     QChartView *chartView;
//     QChart *chartBar =  themeWidget->createSplineChart();
//     chartView = new QChartView(chartBar);

//     tableLayout = new QVBoxLayout(this);
//     tableButtonLayout = new QHBoxLayout(this);


    splitter->addWidget(themeWidget);
    //splitter->addWidget(chartView);
	setCentralWidget(splitter);

//    QItemSelectionModel *selectionModelTree = treeView->selectionModel();
//	QModelIndex rootIx = dirModel->index(0, 0, QModelIndex());//корневой элемент

//	QModelIndex indexHomePath = dirModel->index(homePath);
//	QFileInfo fileInfo = dirModel->fileInfo(indexHomePath);

    QItemSelectionModel *selectionModelTable = tableView->selectionModel();

//	/* Рассмотрим способы обхода содержимого папок на диске.
//	 * Предлагается вариант решения, которы может быть применен для более сложных задач.
//	 * Итак, если требуется выполнить анализ содержимого папки, то необходимо организовать обход содержимого. Обход выполняем относительно модельного индекса.
//	 * Например:*/
//	if (fileInfo.isDir()) {
//		/*
//		 * Если fileInfo папка то заходим в нее, что бы просмотреть находящиеся в ней файлы.
//		 * Если нужно просмотреть все файлы, включая все вложенные папки, то нужно организовать рекурсивный обход.
//		*/
//		QDir dir  = fileInfo.dir();

//		if (dir.cd(fileInfo.fileName())) {
//			/**
//			 * Если зашли в папку, то пройдемся по контейнеру QFileInfoList ,полученного методом entryInfoList,
//			 * */

//			foreach (QFileInfo inf, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Type)) {
//				qDebug() << inf.fileName() << "---" << inf.size();
//			}

//			dir.cdUp();//выходим из папки
//		}
//	}

//	QDir dir = fileInfo.dir();

//	foreach (QFileInfo inf, dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Type)) {

//		qDebug() << inf.fileName() << "---" << inf.size();
//	}


//    treeView->header()->resizeSection(0, 200);
    //tableView->verticalHeader()->resizeSection(0, 2000);
	//Выполняем соединения слота и сигнала который вызывается когда осуществляется выбор элемента в TreeView
//    connect(selectionModelTree, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
//            this, SLOT(selectInTreeSlot(const QItemSelection &, const QItemSelection &)));

    connect(changeDirectoryButton, SIGNAL(clicked()), this, SLOT(openFileDialogWindow()));


    //Выполняем соединения слота и сигнала который вызывается когда осуществляется выбор элемента в TableView
    connect(selectionModelTable, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectInTableSlot(const QItemSelection &, const QItemSelection &)));
    //Пример организации установки курсора в TreeView относительно модельного индекса
    //	QItemSelection toggleSelection;
    //	QModelIndex topLeft;
    //	topLeft = dirModel->index(homePath);
    //	dirModel->setRootPath(homePath);

    //	toggleSelection.select(topLeft, topLeft);
    //	selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}
//Слот для обработки выбора элемента в TreeView
//выбор осуществляется с помощью курсора


void MainWindow::openFileDialogWindow()
{
    QFileDialog fileDialog(this);

    fileDialog.setFileMode(QFileDialog::Directory);
             // Установить путь к файлу по умолчанию
    fileDialog.setDirectory(QDir::homePath());

    QString filePath;
    if(fileDialog.exec())
    {
        filePath = fileDialog.selectedFiles().first();

        tableView->setRootIndex(fileModel->setRootPath(filePath));
    }


}

//void MainWindow::selectInTreeSlot(const QItemSelection &selected, const QItemSelection &deselected)
//{
//	//Q_UNUSED(selected);
//    //Q_UNUSED(deselected);
//	QModelIndex index = treeView->selectionModel()->currentIndex();
//	QModelIndexList indexs =  selected.indexes();
//	QString filePath = "";

//	// Размещаем инфо в statusbar относительно выделенного модельного индекса

//	if (indexs.count() >= 1) {
//		QModelIndex ix =  indexs.constFirst();
//		filePath = dirModel->filePath(ix);
//		this->statusBar()->showMessage("Выбранный путь : " + dirModel->filePath(indexs.constFirst()));
//	}

//	//TODO: !!!!!
//	/*
//	Тут простейшая обработка ширины первого столбца относительно длины названия папки.
//	Это для удобства, что бы при выборе папки имя полностью отображалась в  первом столбце.
//	Требуется доработка(переработка).
//	*/
//	int length = 200;
//	int dx = 30;

//	if (dirModel->fileName(index).length() * dx > length) {
//		length = length + dirModel->fileName(index).length() * dx;
//		qDebug() << "r = " << index.row() << "c = " << index.column() << dirModel->fileName(index) << dirModel->fileInfo(
//					 index).size();

//	}

//	treeView->header()->resizeSection(index.column(), length + dirModel->fileName(index).length());
//	tableView->setRootIndex(fileModel->setRootPath(filePath));
//}

void MainWindow::selectInTableSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    //Q_UNUSED(selected);
    //Q_UNUSED(deselected);
    //QModelIndex index = treeView->selectionModel()->currentIndex();
    QModelIndexList indexs =  selected.indexes();
    QString filePath = "";

    // Размещаем инфо в statusbar относительно выделенного модельного индекса

    if (indexs.count() >= 1) {
        QModelIndex ix =  indexs.constFirst();
        filePath = fileModel->filePath(ix);
        this->statusBar()->showMessage("Выбранный путь : " + fileModel->filePath(indexs.constFirst()));
    }


    QString fileExtension = fileModel->filePath(indexs.constFirst());
    fileExtension.remove(0, fileExtension.indexOf('.'));
    if (addReaderInContainer(fileExtension))
        themeWidget->updateDataGraphic(fileModel->filePath(indexs.constFirst()));
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","This file not support");
        messageBox.setFixedSize(500,200);
    }

    //TODO: !!!!!
    /*
    Тут простейшая обработка ширины первого столбца относительно длины названия папки.
    Это для удобства, что бы при выборе папки имя полностью отображалась в  первом столбце.
    Требуется доработка(переработка).
    */
//	int length = 200;
//	int dx = 30;

//	if (dirModel->fileName(index).length() * dx > length) {
//		length = length + dirModel->fileName(index).length() * dx;
//		qDebug() << "r = " << index.row() << "c = " << index.column() << dirModel->fileName(index) << dirModel->fileInfo(
//					 index).size();

//	}

//	treeView->header()->resizeSection(index.column(), length + dirModel->fileName(index).length());
//	tableView->setRootIndex(fileModel->setRootPath(filePath));
}

MainWindow::~MainWindow()
{

}

bool MainWindow::addReaderInContainer(QString fileExtension)
{
    qDebug() << fileExtension;

    if (fileExtension == ".sqlite")
     {
        IOCContainer::instance().RegisterFactory<IReadData, ReadDataSqlite>();
        return true;
    }

    if (fileExtension == ".json")
    {
        IOCContainer::instance().RegisterFactory<IReadData, ReadDataJson>();
        return true;
    }


    return false;
}
