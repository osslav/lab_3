#include <QtSql>

#include "readdata.h"

//функция для чтения из файла sqlite по его пути в DataList - список данных
bool ReadDataSqlite::readData(DataList& dataList, const QString& filePath)
{
    static QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");     //создаем статическую переменную базы данных spllite
    sdb.setConnectOptions("QSQLITE_OPEN_READONLY=1");                   //работаем с ней в режиме только чтение
    sdb.setDatabaseName(filePath);                                      //передаем путь к файлу объекту базы данных
    if (sdb.open())                                                     //открываем базу данных, если открытие успешно:
    {
        QString dbName = filePath;                                      //выделяем из пути к файлу имя базы данных
        dbName.remove(0,dbName.lastIndexOf('/') + 1);
        dbName.remove(dbName.indexOf('.'), dbName.size() - dbName.indexOf('.'));

        dataList.clear();                                               //очищаем старые данные
        QSqlQuery query("SELECT VALUE, TIME FROM " + dbName, sdb);      //создаем запрос к базе данных
        int index = 0;                                                  //индекс текущего элемента в базе данных


        while (query.next())                                            //цикл идет по элементам базы данных
        {
            double tempVal = query.value(0).toDouble();                 //считываем текущее значение и дату из элемента базы данных
            QString tempDate = query.value(1).toString();
            QPointF value(index, tempVal);

            dataList << Data(value, tempDate);                          //записываем их в список данных

            index++;
        }

        sdb.close();                                                    //закрываем базу данных

        return true;                                                    //выходим из функции
    }
    else                                                                //если открытие неуспешно, то возвращаем false
    {
        return false;
    }
}

//функция для чтения из файла json по его пути в DataList - список данных
bool ReadDataJson::readData(DataList& dataList, const QString& filePath)
{
    QString val;                //строка - будет хранить содержимое файла json
    QFile file;                 //файл - будет содержать необходимый json файл
    file.setFileName(filePath); //устанавливаем для файла переданный в функцию путь
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  //открываем файл
        return false;               //если он не открылся - ошибка - завершаем фукнцию

    val = file.readAll();       //читаем содержимое файла в строку
    file.close();               //закрываем файл

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());  //создаем json документ с данными из файла

    if (!doc.isArray())         //если json документ не содержит массив - выходим из функции, т к по нему не предусмотрен вариант построения графика
    {
        return false;
    }
    QJsonArray arr = doc.array();   //получаем json массив из документа

    dataList.clear();           //очищаем старые данные из списка
    int index = 0;              //индекс элементов массива
    foreach (const QJsonValue & value, arr)     //цикл идет по элементам json массива
    {
        if (value.isObject())   //если элемент массива - объект, то
        {
            QJsonObject obj = value.toObject();     //формируем этот объект
            double val = obj["Value"].toDouble();   //считываем из него значение и дату в виде строки
            QString time = obj["Time"].toString();

            QPointF point(index, val);              //заносим эти данные в список
            dataList << Data(point, time);

            index++;                                //обновляем индекс текущего элемента
        }
    }

    return true;
}
