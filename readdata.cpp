#include <QtSql>

#include "readdata.h"

bool ReadDataSqlite::readData(DataList& dataList, const QString& filePath)
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setConnectOptions("QSQLITE_OPEN_READONLY=1");
    sdb.setDatabaseName(filePath);
    if (sdb.open())
    {
        QString dbName = filePath;
        dbName.remove(0,dbName.lastIndexOf('/') + 1);
        dbName.remove(dbName.indexOf('.'), dbName.size() - dbName.indexOf('.'));

        dataList.clear();
        QSqlQuery query("SELECT VALUE, TIME FROM " + dbName, sdb);
        int index = 0;
        qDebug() << dbName;


        while (query.next())
        {
            double tempVal = query.value(0).toDouble();
            QString tempDate = query.value(1).toString();
            QPointF value(index, tempVal);

            dataList << Data(value, tempDate);

            index++;
        }

        return true;
    }
    else
    {
        return false;
    }

}

bool ReadDataJson::readData(DataList& dataList, const QString& filePath)
{
    QString val;
    QFile file;
    file.setFileName(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    val = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());

    if (!doc.isArray())
    {
        return false;
    }
    QJsonArray arr = doc.array();

    dataList.clear();
    int index = 0;
    foreach (const QJsonValue & value, arr)
    {
        if (value.isObject())
        {
            QJsonObject obj = value.toObject();
            double val = obj["Value"].toDouble();
            QString time = obj["Time"].toString();

            QPointF point(index, val);
            dataList << Data(point, time);

            index++;
        }
    }

}
