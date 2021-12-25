#include "headers/pass_recovery.h"
#include "ui_pass_recovery.h"
#include <QMessageBox>

pass_recovery::pass_recovery(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::pass_recovery) {
    ui->setupUi(this);
}

pass_recovery::~pass_recovery() {
    delete ui;
}

//кнопка назад
void pass_recovery::on_pushButton_clicked() {
    this->close();
    clear_window();
    emit firstWindow();
}

//кнопка восстановить пароль
void pass_recovery::on_pushButton_2_clicked() {
    ui->pass_message->clear();
    checker ch;
    QString fname = QString(
            QCryptographicHash::hash(ui->lineEdit->text().toUtf8(), QCryptographicHash::Sha256).toHex());
    QString sname = QString(
            QCryptographicHash::hash(ui->lineEdit_2->text().toUtf8(), QCryptographicHash::Sha256).toHex());
    QString birth = ui->lineEdit_3->text();
    QString login = ui->lineEdit_4->text();

    QString email = QString(
            QCryptographicHash::hash(ui->lineEdit_5->text().toUtf8(), QCryptographicHash::Sha256).toHex());
    QString npas1 = QString(
            QCryptographicHash::hash(ui->lineEdit_6->text().toUtf8(), QCryptographicHash::Sha256).toHex());
    QString npas2 = QString(
            QCryptographicHash::hash(ui->lineEdit_7->text().toUtf8(), QCryptographicHash::Sha256).toHex());
    if (!fname.isEmpty() && !sname.isEmpty() && !birth.isEmpty() &&
        !login.isEmpty() && !email.isEmpty() && !npas1.isEmpty() && !npas2.isEmpty()) {
        if (!ch.Login_check(login)) {
            ui->pass_message->setText("Incorrect Login format");
            return;
        }
        if (npas1 == npas2) {
            DataBase::Get_db().connectToDataBase();
            if (DataBase::check_person(login, npas1)) {
                ui->pass_message->setText("The new password must be different from the old one");
                return;
            }
            if (DataBase::full_person_check(fname, sname, birth, login, email)) {
                std::thread th([login, npas1]() {
                    DataBase::Get_db().connectToDataBase();
                    DataBase::change_password(login, npas1);
                });
                th.detach();
                QMessageBox::StandardButton button = QMessageBox::information(this, "Password recovery",
                                                                              "You have successfully change your password",
                                                                              QMessageBox::Ok);
                if (button == QMessageBox::Ok) {
                    close();
                    clear_window();
                    emit firstWindow();
                    emit lost_connetcion();
                }
            } else {
                QMessageBox::information(this, "Attention", "Incorrect Data");
            }
        } else {
            ui->pass_message->setText("Passwords are not the same");
            return;
        }
    } else {
        QMessageBox::information(this, "Attention", "Please fill in all empty lines");
    }
}


void pass_recovery::on_checkBox_stateChanged(int) {
    if (ui->checkBox->isChecked()) {
        ui->lineEdit_6->setEchoMode(QLineEdit::Normal);
    } else {
        ui->lineEdit_6->setEchoMode(QLineEdit::Password);
    }
}

void pass_recovery::on_checkBox_2_stateChanged(int) {
    if (ui->checkBox_2->isChecked()) {
        ui->lineEdit_7->setEchoMode(QLineEdit::Normal);
    } else {
        ui->lineEdit_7->setEchoMode(QLineEdit::Password);
    }
}

void pass_recovery::clear_window() {
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->setDate(ui->lineEdit_3->minimumDate());
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_7->clear();
    ui->pass_message->clear();
}

