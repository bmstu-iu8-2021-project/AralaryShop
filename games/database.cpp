#include "database.h"
#include <iostream>

DataBase::DataBase(QObject *parent) : QObject(parent) {}

DataBase::~DataBase() { closeDataBase(); }

/* Методы для подключения к базе данных
 * */
void DataBase::connectToDataBase() {
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её
     * восстановление
     * */
    if (!QFile("C:\\course_project\\" + DATABASE_NAME).exists()) {
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}



bool DataBase::game_exist(const QString &gname) {
    QSqlQuery query;
    query.prepare("SELECT * FROM" + TABLE + "WHERE Gname = :gn");
    query.bindValue(":gn", gname);
    if (!query.exec()) {
        return false;
    } else {
        return query.next();
    }
    return false;
}

void DataBase::set_table(const QString &table) { TABLE = table; }

bool DataBase::createTable(int)
{
    QSqlQuery query;
    if (!query.exec("CREATE TABLE " + TABLE +
                    " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "Gname VARCHAR(255)    NOT NULL,"
                    "Price VARCHAR(255)    NOT NULL,"
                    "Genre VARCHAR(255)    NOT NULL,"
                    "Age VARCHAR(255)    NOT NULL,"
                    "Date VARCHAR(255)    NOT NULL"
                    " )")) {
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


/* Методы восстановления базы данных
 * */
bool DataBase::restoreDataBase() {
    // Если база данных открылась ...
    if (this->openDataBase()) {
        // Производим восстановление базы данных
        return (this->createTable()) ? true : false;
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

/* Метод для открытия базы данных
 * */
bool DataBase::openDataBase() {
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName("C:\\course_project\\" + DATABASE_NAME);
    if (db.open()) {
        return true;
    } else {
        return false;
    }
}

/* Методы закрытия базы данных
 * */
void DataBase::closeDataBase() { db.close(); }

/* Метод для создания таблицы в базе данных
 * */
bool DataBase::createTable() {
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением.
     * */
    QSqlQuery query;
    if (!query.exec("CREATE TABLE " + TABLE +
                    " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "Gname VARCHAR(255)    NOT NULL,"
                    "Price VARCHAR(255)    NOT NULL,"
                    "Genre VARCHAR(255)    NOT NULL,"
                    "Age VARCHAR(255)    NOT NULL,"
                    "Date VARCHAR(255)    NOT NULL"
                    " )")) {
        qDebug() << "DataBase: error of create " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

/* Метод для вставки записи в базу данных
 * */
bool DataBase::inserIntoTable(const QVariantList &data) {
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " + TABLE +
                  " ( "
                  "Gname"
                  ", "
                  "Price"
                  ", "
                  "Genre"
                  ", "
                  "Age"
                  ", "
                  "Date"
                  " ) "
                  "VALUES (:gn, :pr, :ge, :age, :date)");
    query.bindValue(":gn", data[0].toString());
    query.bindValue(":pr", data[1].toString());
    query.bindValue(":ge", data[2].toString());
    query.bindValue(":age", data[3].toString());
    query.bindValue(":date", data[4].toString());
    // После чего выполняется запросом методом exec()
    if (!query.exec()) {
        qDebug() << "error insert into " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

/* Второй метод для вставки записи в базу данных
 * */
bool DataBase::inserIntoTable(const QString &gname, const QString &price,
                              const QString &genre, const QString &age,
                              const QString &date) {
    QVariantList data;
    data.append(gname);
    data.append(price);
    data.append(genre);
    data.append(age);
    data.append(date);

    if (inserIntoTable(data))
        return true;
    else
        return false;
}

/* Метод для удаления записи из таблицы
 * */
bool DataBase::removeRecord(const int id) {
    // Удаление строки из базы данных будет производитсья с помощью SQL-запроса
    QSqlQuery query;

    // Удаление производим по id записи, который передается в качестве аргумента
    // функции
    query.prepare("DELETE FROM " + TABLE + " WHERE id= :ID ;");
    query.bindValue(":ID", id);

    // Выполняем удаление
    if (!query.exec()) {
        qDebug() << "error delete row " << TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}
