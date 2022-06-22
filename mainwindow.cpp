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
    tableWidget = new MyTable();
    //Устанавливаем размер главного окна
    this->setGeometry(100, 100, 1500, 500);
	this->setStatusBar(new QStatusBar(this));
	this->statusBar()->showMessage("Choosen Path: ");


	QSplitter *splitter = new QSplitter(parent);

    splitter->addWidget(tableWidget);
    splitter->addWidget(themeWidget);

    splitter->addWidget(themeWidget);
	setCentralWidget(splitter);

    connect(tableWidget, SIGNAL(directoryChanged()), this, SLOT(selectInTableSlot()));
}

void MainWindow::selectInTableSlot()
{
    QString filePath = tableWidget->getDirectory();

    // Размещаем инфо в statusbar относительно выделенного модельного индекса
    this->statusBar()->showMessage("Выбранный путь : " + filePath);

    themeWidget->updateDataGraphic(filePath);
}


MainWindow::~MainWindow()
{

}



MyTable::MyTable(QWidget *parent)
    : QWidget(parent)
{
    QString homePath = QDir::homePath();

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);

    fileModel->setRootPath(homePath);

    tableLayout = new QVBoxLayout(this);
    tableView = new QTableView;
    changeDirectoryButton = new QPushButton("Change directory");
    tableLayout->addWidget(tableView);
    tableLayout->addWidget(changeDirectoryButton);
    tableView->setModel(fileModel);

    QItemSelectionModel *selectionModelTable = tableView->selectionModel();

    connect(changeDirectoryButton, SIGNAL(clicked()), this, SLOT(changeDirectory()));

    connect(selectionModelTable, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(selectInTableSlot(const QItemSelection &, const QItemSelection &)));
}

void MyTable::changeDirectory()
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



void MyTable::selectInTableSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    QModelIndexList indexs =  selected.indexes();
    QString filePath = "";

    if (indexs.count() >= 1)
    {
        QModelIndex ix =  indexs.constFirst();
        filePath = fileModel->filePath(ix);
    }


    QString fileExtension = fileModel->filePath(indexs.constFirst());
    fileExtension.remove(0, fileExtension.indexOf('.'));
    if (addReaderInContainer(fileExtension))
    {
        currentPath = fileModel->filePath(indexs.constFirst());
        emit directoryChanged();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","This file not support");
        messageBox.setFixedSize(500,200);
    }
}

bool MyTable::addReaderInContainer(QString fileExtension)
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
