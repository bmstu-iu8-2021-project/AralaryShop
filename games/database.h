#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

/* Директивы имен таблицы, полей таблицы и базы данных */
//#define DATABASE_HOSTNAME   "host1488"
//#define DATABASE_NAME       "users_data.db"

//#define TABLE                   "Users"         // Название таблицы
// Первая колонка содержит Autoincrement ID

class DataBase : public QObject {
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);

    ~DataBase();

    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    void connectToDataBase();

    bool game_exist(const QString &gname);

    void set_table(const QString &table);

    bool createTable(int);

private:
    /* Внутренние методы для работы с базой данных
     * */

    bool openDataBase();        // Открытие базы данных
    bool restoreDataBase();     // Восстановление базы данных
    void closeDataBase();       // Закрытие базы данных
    bool createTable();         // Создание базы таблицы в базе данных

public
    slots:
    bool inserIntoTable(const QVariantList &data);      // Добавление записей в таблицу
    bool inserIntoTable(const QString &gname, const QString &price,
                        const QString &genre, const QString &age, const QString &date);

    bool removeRecord(const int id); // Удаление записи из таблицы по её id

private:
    // Сам объект базы данных, с которым будет производиться работа
    QString DATABASE_HOSTNAME = "host1488";
    QString DATABASE_NAME = "users_data.db";
    QString TABLE = "Games";
    QSqlDatabase db;
};


#endif // DATABASE_H
