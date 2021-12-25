#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->label->clear();

    db = &DataBase::Get_db();
    db->connectToDataBase();
    reg_window = new registration();
    connect(reg_window, &registration::firstWindow, this, &MainWindow::show);
    connect(reg_window, &registration::lost_connection, this, &MainWindow::refresh_connection);

    rec_window = new pass_recovery();
    connect(rec_window, &pass_recovery::firstWindow, this, &MainWindow::show);
    connect(reg_window, &registration::lost_connection, this, &MainWindow::refresh_connection);

    shop_window = new Shop();
    connect(shop_window, &Shop::firstWindow, this, &MainWindow::show);
    connect(this, &MainWindow::sent_person, shop_window, &Shop::set_person);
}

MainWindow::~MainWindow() {
    delete rec_window;
    delete reg_window;
    delete shop_window;
    delete ui;
}

void MainWindow::refresh_connection() {
    DataBase::Get_db().connectToDataBase();
}

void MainWindow::on_reg_button_clicked() {
    clear_line();
    reg_window->show();
    hide();
}

//checkbox (показать или скрыть пароль)
void MainWindow::on_show_pass0_stateChanged(int) {
    if (ui->show_pass0->isChecked()) {
        ui->password->setEchoMode(QLineEdit::Normal);
    } else {
        ui->password->setEchoMode(QLineEdit::Password);
    }
}


void MainWindow::clear_line() {
    ui->password->clear();
    ui->login->clear();
    ui->label->clear();
}

//кнопка войти
void MainWindow::on_enter_button_clicked() {
    ui->label->clear();
    checker ch;
    QString log = ui->login->text();
    QString pas = QString(QCryptographicHash::hash(ui->password->text().toUtf8(), QCryptographicHash::Sha256).toHex());
    if (!ch.Login_check(log)) {
        ui->label->setText("Incorrect login or password");
        return;
    }

    if (db->check_person(log, pas)) {
        clear_line();
        close();
        shop_window->show();
        emit sent_person(log);
    } else {
        ui->label->setText("Incorrect login or password");
    }
}

//конопка забыл пароль
void MainWindow::on_refresh_pass_button_clicked() {
    clear_line();
    hide();
    rec_window->show();
}



