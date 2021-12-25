#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "registration.h"
#include "shop.h"
#include "pass_recovery.h"
#include "database.h"
#include "checker.h"
#include <thread>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

public
    slots:

            void refresh_connection();

private
    slots:

            void on_reg_button_clicked();

    void on_show_pass0_stateChanged(int);

    void on_enter_button_clicked();

    void on_refresh_pass_button_clicked();


    signals:

            void sent_db(DataBase * DB);

    void sent_person(const QString &login);

private:

    void clear_line();

    bool f = 1;
    DataBase *db;
    Ui::MainWindow *ui;
    registration *reg_window;
    Shop *shop_window;
    pass_recovery *rec_window;
};
#endif // MAINWINDOW_H
