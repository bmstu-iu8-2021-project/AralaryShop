#ifndef SHOP_H
#define SHOP_H

#include <QMainWindow>
#include "database.h"
#include "money.h"
#include <QSqlQueryModel>
#include "cur_game.h"
#include <QMessageBox>
#include "users_list.h"
#include <thread>
#include <QThread>

namespace Ui {
    class Shop;
}

class Shop : public QMainWindow {
    Q_OBJECT

public:
    explicit Shop(QWidget *parent = nullptr);

    ~Shop();

    signals:
            void firstWindow();

    void sent_login(const QString &login, const QString &money);

    void sent_game(const QString &login, const QString &gname);

    void get_users_list(const QString &login);

    void lost_connection();

public
    slots:
            void set_person(
    const QString &login
    );

    void set_status();

    void refreshrer();

private
    slots:
            void on_pushButton_clicked();


    void on_pushButton_4_clicked();


    void on_pushButton_2_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_pushButton_6_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_5_clicked();

private:
    bool add_money_Flag = 0;
    users_list *ulist;
    cur_game *temp_game;
    QSqlQueryModel *model;
    money *m_window;
    QString LOGIN;
    Ui::Shop *ui;
};

#endif // SHOP_H
