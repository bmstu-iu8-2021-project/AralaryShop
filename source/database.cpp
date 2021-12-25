#include "headers/database.h"
#include <iostream>

DataBase &DataBase::Get_db() {
    static DataBase a;

    return a;
}

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

bool DataBase::check_person(const QString &login, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE Login = :Log AND Password = :Pas");
    query.bindValue(":Log", login);
    query.bindValue(":Pas", password);
    if (!query.exec()) {
        return false;
    } else {
        return query.next();
    }
    return false;
}

void DataBase::change_password(const QString &login, const QString &password) {
    QSqlQuery query;
    query.prepare("UPDATE Users SET Password = :Pas WHERE Login = :Log");
    query.bindValue(":Log", login);
    query.bindValue(":Pas", password);
    query.exec();
}

void DataBase::change_phone(const QString &login, const QString &phone) {
    QSqlQuery query;
    query.prepare("UPDATE Users SET Phone = :Ph WHERE Login = :Log");
    query.bindValue(":Log", login);
    query.bindValue(":Ph", phone);
    query.exec();
}

bool DataBase::person_exist(const QString &login) {
    QSqlQuery query;
    query.prepare("SELECT * FROM Users WHERE Login = :Log");
    query.bindValue(":Log", login);
    if (!query.exec()) {
        return false;
    } else {
        return query.next();
    }
    return false;
}

bool DataBase::game_check(const QString &login, const QString &gname) {
    QSqlQuery query;
    query.prepare("SELECT * FROM person_games WHERE Login = :Log AND Gname = :Game");
    query.bindValue(":Log", login);
    query.bindValue(":Game", gname);
    if (!query.exec()) {
        return false;
    } else {
        return query.next();
    }
    return false;
}

QString DataBase::phone(const QString &login) {
    QSqlQuery query;
    query.prepare("Select Phone FROM Users WHERE Login = :Log");
    query.bindValue(":Log", login);
    query.exec();
    query.first();
    QString phone = query.value(0).toString();
    return phone;
}

QString DataBase::get_BirthDate(const QString &login) {
    QSqlQuery query;
    query.prepare("Select Birth FROM Users WHERE Login = :Log");
    query.bindValue(":Log", login);
    query.exec();
    query.first();
    QString phone = query.value(0).toString();
    return phone;
}

void DataBase::change_favourite(const QString &login, const QString &game) {
    QSqlQuery query;
    query.prepare("UPDATE person_games SET IsFavourite = :flag WHERE Login = :log AND Gname = :gname");
    bool f = !(get_favourite(login, game));
    query.bindValue(":flag", f);
    query.bindValue(":log", login);
    query.bindValue(":gname", game);
    query.exec();
}

void DataBase::set_table(const QString &table) { TABLE = table; }

void DataBase::add_money(const QString &login, const QString &add_cash, const QString &old_cash) {

    QSqlQuery query;
    QString new_cash;
    double old = old_cash.toDouble();
    double add = add_cash.toDouble();
    std::ostringstream stream;
    stream << (old + add);
    std::string res = stream.str();
    new_cash = QString::fromStdString(res);
    query.prepare("UPDATE Users SET Money = :Cash WHERE Login = :Log");
    query.bindValue(":Log", login);
    query.bindValue(":Cash", new_cash);
    query.exec();
}

void DataBase::reduce_money(const QString &login, const QString &red_cash, const QString &old_cash) {
    QSqlQuery query;
    QString new_cash;
    double old = old_cash.toDouble();
    double red = red_cash.toDouble();
    std::ostringstream stream;
    stream << (old - red);
    std::string res = stream.str();
    new_cash = QString::fromStdString(res);
    query.prepare("UPDATE Users SET Money = :Cash WHERE Login = :Log");
    query.bindValue(":Log", login);
    query.bindValue(":Cash", new_cash);
    query.exec();
}

QString DataBase::get_money(const QString &login) {
    QSqlQuery query;
    query.prepare("Select Money FROM Users WHERE Login = :Log");
    query.bindValue(":Log", login);
    query.exec();
    query.first();
    double cash = query.value(0).toDouble();
    std::ostringstream stream;
    stream << cash;
    std::string res = stream.str();
    return QString::fromStdString(res);
}

bool DataBase::get_favourite(const QString &login, const QString &gname) {
    QSqlQuery query;
    query.prepare("Select IsFavourite FROM person_games WHERE Login = :Log AND Gname = :gname");
    query.bindValue(":Log", login);
    query.bindValue(":gname", gname);

    if (query.exec()) {
        query.first();
        return query.value(0).toBool();
    } else {
        qDebug() << "error";
        return false;
    }
}

bool DataBase::full_person_check(const QString &fname, const QString &sname,
                                 const QString &birth, const QString &login,
                                 const QString &email) {
    QSqlQuery query;
    query.prepare("SELECT id FROM Users WHERE Login = :Log AND Fname = :Fn AND "
                  "Sname = :Sn AND Birth = :Bi AND Email = :Em");
    query.bindValue(":Log", login);
    query.bindValue(":Fn", fname);
    query.bindValue(":Sn", sname);
    query.bindValue(":Bi", birth);
    query.bindValue(":Em", email);

    if (!query.exec()) {
        return false;
    } else {
        return query.next();
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
    }

    return false;

}

/* Метод закрытия базы данных
 * */
void DataBase::closeDataBase() {
    db.close();
}

/* Метод для создания таблицы в базе данных
 * */

bool DataBase::createTable(const QString &table) {
    if (table == "person_games") {
        QSqlQuery query;
        if (!query.exec("CREATE TABLE " + table +
                        " ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "Login VARCHAR(255)    NOT NULL,"
                        "Gname VARCHAR(255)    NOT NULL,"
                        "IsFavourite BOOL"
                        " )")) {
            qDebug() << "DataBase: error of create " << table;
            qDebug() << query.lastError().text();
            return false;
        } else {
            return true;
        }
    }
    return false;
}

bool DataBase::createTable() {
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением.
     * */
    QSqlQuery query;
    if (!query.exec("CREATE TABLE " + TABLE +
                    " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "Login VARCHAR(255)    NOT NULL,"
                    "Password VARCHAR(255)    NOT NULL,"
                    "Phone VARCHAR(255)    NOT NULL,"
                    "Money VARCHAR(255)    NOT NULL,"
                    "Fname VARCHAR(255)    NOT NULL,"
                    "Sname VARCHAR(255)    NOT NULL,"
                    "Birth VARCHAR(255)    NOT NULL,"
                    "Email VARCHAR(255)    NOT NULL"
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
bool DataBase::inserIntoTable(const QString &table, const QVariantList &data) {
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    if (table == "person_games") {
        QSqlQuery query;
        /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
        query.prepare("INSERT INTO " + table +
                      " ( "
                      "Login"
                      ", "
                      "Gname"
                      ", "
                      "IsFavourite"
                      " ) "
                      "VALUES (:Login, :Game, :Fav)");
        query.bindValue(":Login", data[0].toString());
        query.bindValue(":Game", data[1].toString());
        query.bindValue(":Fav", false);

        // После чего выполняется запросом методом exec()
        if (!query.exec()) {
            qDebug() << "error insert into " << table;
            qDebug() << query.lastError().text();
            return false;
        }

        return true;
    } else if (table == "Users") {
        QSqlQuery query;
        /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
        query.prepare("INSERT INTO " + table +
                      " ( "
                      "Login"
                      ", "
                      "Password"
                      ", "
                      "Phone"
                      ", "
                      "Money"
                      ", "
                      "Fname"
                      ", "
                      "Sname"
                      ", "
                      "Birth"
                      ", "
                      "Email"
                      " ) "
                      "VALUES (:Login, :Password, :Phone, :Money, :Fname, :Sname, :Birth, :Email)");
        query.bindValue(":Login", data[0].toString());
        query.bindValue(":Password", data[1].toString());
        query.bindValue(":Phone", "");
        query.bindValue(":Money", "0");
        query.bindValue(":Fname", data[2].toString());
        query.bindValue(":Sname", data[3].toString());
        query.bindValue(":Birth", data[4].toString());
        query.bindValue(":Email", data[5].toString());
        // После чего выполняется запросом методом exec()
        if (!query.exec()) {
            qDebug() << "error insert into " << table;
            qDebug() << query.lastError().text();
            return false;
        } else {
            return true;
        }
    }
    return false;
}

/* Второй метод для вставки записи в базу данных
 * */
bool DataBase::inserIntoTable(const QString &login, const QString &password,
                              const QString &fname, const QString &sname,
                              const QString &birth, const QString &email) {
    QVariantList data;
    data.append(login);
    data.append(password);
    data.append(fname);
    data.append(sname);
    data.append(birth);
    data.append(email);
    if (inserIntoTable("Users", data)) {
        return true;

    }
    return false;
}

bool DataBase::inserIntoTable(const QString &login, const QString &gname) {
    QVariantList data;
    data.append(login);
    data.append(gname);

    if (inserIntoTable("person_games", data)) {
        return true;
    }
    return false;
}
