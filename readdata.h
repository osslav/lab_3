#ifndef READDATA_H
#define READDATA_H

#include <QObject>

using Data = QPair<QPointF, QString>;
using DataList = QList<Data>;
using DataTable = QList<DataList>;


enum TypeFiles
{
    Sqlite,
    Json
};

class IReadData
{
public:
    IReadData() {};

    virtual void readData(DataTable& data, const QString& filePath) = 0;
};


class ReadDataSqlite : public IReadData
{
public:
    ReadDataSqlite() {};

    virtual void readData(DataTable& dataTable, const QString& filePath);
};

class ReadDataJson : public IReadData
{
public:
    ReadDataJson() {};

    virtual void readData(DataTable& dataTable, const QString& filePath);
};

#endif // READDATA_H
