#include <QtSql>

#include "readdata.h"

//функция для чтения из файла sqlite по его пути в DataList - список данных
QString ReadDataSqlite::readData(DataList& dataList, const QString& filePath)
{
    static QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");     //создаем статическую переменную базы данных spllite
    DataList newDataList;                                               //временное хранилище новых данных
    sdb.setConnectOptions("QSQLITE_OPEN_READONLY=1");                   //работаем с ней в режиме только чтение
    sdb.setDatabaseName(filePath);                                      //передаем путь к файлу объекту базы данных
    if (sdb.open())                                                     //открываем базу данных, если открытие успешно:
    {
        QString dbName = filePath;                                      //выделяем из пути к файлу имя базы данных
        dbName.remove(0,dbName.lastIndexOf('/') + 1);
        dbName.remove(dbName.indexOf('.'), dbName.size() - dbName.indexOf('.'));

        QSqlQuery query("SELECT VALUE, TIME FROM " + dbName, sdb);      //создаем запрос к базе данных
        int index = 0;                                                  //индекс текущего элемента в базе данных

        while (query.next())                                            //цикл идет по элементам базы данных
        {
                                                                        //проверяем пригодность получаемых данных к использованию
            if (query.value(0).userType() != QMetaType::Double || query.value(1).userType() != QMetaType::QString)
            {
                sdb.close();                                            //если полученные данные не Double или QString - закрываем бд и выходим из функции
                return ("Данные не в нужном формате. Файл: " + filePath);
            }

            double tempVal = query.value(0).toDouble();                 //считываем текущее значение и дату из элемента базы данных
            QString tempDate = query.value(1).toString();
            QPointF value(index, tempVal);

            newDataList << Data(value, tempDate);                       //записываем их в список данных

            index++;
        }

        sdb.close();                                                    //закрываем базу данных

        if (index == 0)                                                 //отсутствие пригодных данных тоже считается нетипичной проблемой и требует сообщения пользователю
        {
            return ("Необходимые данные не найдены. Файл: " + filePath);
         }


        dataList = newDataList;                                         //записываем новые данные
        return "";                                                    //выходим из функции(успешное выполнение)
    }
    else                                                                //если открытие файла неуспешно, то возвращаем сообщение об ошибке
    {
        return ("Ошибка открытия файла " + filePath);
    }
}

//функция для чтения из файла json по его пути в DataList - список данных
QString ReadDataJson::readData(DataList& dataList, const QString& filePath)
{
    DataList newDataList;       //временное хранилище считываемых данных
    QString val;                //строка - будет хранить содержимое файла json
    QFile file;                 //файл - будет содержать необходимый json файл
    file.setFileName(filePath); //устанавливаем для файла переданный в функцию путь
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))  //открываем файл
        return ("Ошибка открытия файла "  + filePath);               //если он не открылся - ошибка - завершаем фукнцию

    val = file.readAll();       //читаем содержимое файла в строку
    file.close();               //закрываем файл

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());  //создаем json документ с данными из файла

    if (!doc.isArray())         //если json документ не содержит массив - выходим из функции, т к по нему не предусмотрен вариант построения графика
    {
        return ("Данные не представлены в виде массива. Файл: " + filePath);
    }
    QJsonArray arr = doc.array();   //получаем json массив из документа

    int index = 0;              //индекс элементов массива
    foreach (const QJsonValue & value, arr)     //ляю идет по элементам json массива
    {
        if (value.isObject())   //если элемент массива - объект, то
        {
            QJsonObject obj = value.toObject();     //формируем этот объект

            if (obj["Value"].isUndefined() || obj["Value"].isUndefined())       //проверяем существуют ли требуемые данные
            {
                return ("Необходимые данные не найдены. Файл: " + filePath);
            }

            if (!obj["Value"].isDouble() || !obj["Time"].isString())            //проверяем в каком формате представлены полученные данные
            {
                return ("Данные не в нужном формате. Файл: " + filePath);
            }

            double val = obj["Value"].toDouble();   //считываем из него значение и дату в виде строки
            QString time = obj["Time"].toString();

            QPointF point(index, val);              //заносим эти данные в список
            newDataList << Data(point, time);

            index++;                                //обновляем индекс текущего элемента
        }
    }

    dataList = newDataList;             //записываем полученные данные
    return "";                          //выходим из функции
}
