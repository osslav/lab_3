#include <QtSql>

#include "readdata.h"

void ReadDataSqlite::readData(DataTable& dataTable, const QString& filePath)
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setConnectOptions("QSQLITE_OPEN_READONLY=1");
    sdb.setDatabaseName(filePath);
    if (sdb.open())
    {
        QString dbName = filePath;
        dbName.remove(0,dbName.lastIndexOf('/') + 1);
        dbName.remove(dbName.indexOf('.'), dbName.size() - dbName.indexOf('.'));

        dataTable.clear();
        QSqlQuery query("SELECT VALUE, TIME FROM " + dbName, sdb);
        int index = 0;
        DataList dataList;
        qDebug() << dbName;


        while (query.next())
        {
            float tempVal = query.value(0).toFloat();
            QString tempDate = query.value(1).toString();
            QPointF value(index, tempVal);

            dataList << Data(value, tempDate);

            index++;
        }
        dataTable << dataList;
    }

}

void ReadDataJson::readData(DataTable& dataTable, const QString& filePath)
{
/*
    QSqlDatabase sdb = QSqlDatabase::addDatabase("JSON");
    sdb.setConnectOptions("QSQLITE_OPEN_READONLY=1");
    sdb.setDatabaseName(filePath);
    if (sdb.open())
    {
        qDebug() << "BD open\n";
        qDebug() << sdb.connectionName();
        qDebug() << filePath;

        QString dbName = filePath;
        dbName.remove(0,dbName.lastIndexOf('/') + 1);
        dbName.remove(dbName.indexOf('.'), dbName.size() - dbName.indexOf('.'));

        dataTable.clear();
        QSqlQuery query("SELECT VALUE, TIME FROM " + dbName, sdb);
        int index = 0;
        DataList dataList;
        qDebug() << dbName;


        while (query.next())
        {
            float tempVal = query.value(0).toFloat();
            QString tempDate = query.value(1).toString();
            QPointF value(index, tempVal);

            dataList << Data(value, tempDate);

            index++;
        }
        dataTable << dataList;
    }
*/
}
