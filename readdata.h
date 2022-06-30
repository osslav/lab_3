#ifndef READDATA_H
#define READDATA_H

#include <QObject>
#include "printchart.h"

//класс-интерфейс, который считывает данные из файла
class IReadData
{
public:
    //чистая виртуальная функция для чтения из файла по его пути в DataList - список данных
    virtual QString readData(DataList& dataList, const QString& filePath) = 0;
};

//реализация класса IReadData, который считывает данные из файла sqlite
class ReadDataSqlite : public IReadData
{
public:
    //виртуальная функция для чтения из файла sqlite по его пути в DataList - список данных
    virtual QString readData(DataList& dataList, const QString& filePath);
};

//реализация класса IReadData, который считывает данные из файла json
class ReadDataJson : public IReadData
{
public:
    //виртуальная функция для чтения из файла json по его пути в DataList - список данных
    virtual QString readData(DataList& dataList, const QString& filePath);
};

#endif // READDATA_H
