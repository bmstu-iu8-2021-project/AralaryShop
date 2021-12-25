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
#include <sstream>
#include <string>
#include <QCryptographicHash>
#include <mutex>
#include <thread>
#include <atomic>
#include <QThread>

//#define DATABASE_HOSTNAME   "host1488"
//#define DATABASE_NAME       "users_data.db"

//"Users"         // Название таблицы
//"Login"         // Вторая колонка
//"Password"      // Третья колонка
//"Fname"         // Четвертая колонка
//"Sname"         // Пятая колонка
//"Birth"         // Шестая колонка
//"Email"         // Седьмая колонка
// Первая колонка содержит Autoincrement ID

class DataBase : public QObject {
    Q_OBJECT
public:
    static DataBase &Get_db();


    ~DataBase();

    void connectToDataBase();

    static bool check_person(const QString &login, const QString &password);

    static void change_password(const QString &login, const QString &password);

    static void change_phone(const QString &login, const QString &phone);

    static void change_favourite(const QString &login, const QString &game);

    static bool person_exist(const QString &login);

    static bool
    full_person_check(const QString &fname, const QString &sname, const QString &birth, const QString &login,
                      const QString &email);

    static bool game_check(const QString &login, const QString &gname);

    void set_table(const QString &table);

    static void add_money(const QString &login, const QString &add_cash, const QString &old_cash);

    static void reduce_money(const QString &login, const QString &red_cash, const QString &old_cash);

    static QString get_money(const QString &login);

    static bool get_favourite(const QString &login, const QString &gname);

    static QString phone(const QString &login);

    static QString get_BirthDate(const QString &login);

    bool createTable(const QString &table);

    void closeDataBase();       // Закрытие базы данных
    bool createTable();         // Создание базы таблицы в базе данных
private:

    bool openDataBase();        // Открытие базы данных

    bool restoreDataBase();     // Восстановление базы данных

public
    slots:

    static bool inserIntoTable(const QString &table, const QVariantList &data);      // Добавление записей в таблицу


    static bool inserIntoTable(const QString &login, const QString &password,
                               const QString &fname, const QString &sname, const QString &birth, const QString &email);

    static bool inserIntoTable(const QString &login, const QString &gname);

private:
    explicit DataBase(QObject *parent = 0);

    DataBase(const DataBase &db) = delete;

    DataBase &operator=(const DataBase &) = delete;

    QString DATABASE_HOSTNAME = "host1488";
    QString DATABASE_NAME = "users_data.db";
    QString TABLE = "Users";
    QSqlDatabase db;
};

#endif // DATABASE_H
