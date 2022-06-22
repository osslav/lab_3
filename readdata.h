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

    virtual bool readData(DataTable& data, const QString& filePath) = 0;
};


class ReadDataSqlite : public IReadData
{
public:
    ReadDataSqlite() {};

    virtual bool readData(DataTable& dataTable, const QString& filePath);
};

class ReadDataJson : public IReadData
{
public:
    ReadDataJson() {};

    virtual bool readData(DataTable& dataTable, const QString& filePath);
};

#endif // READDATA_H
