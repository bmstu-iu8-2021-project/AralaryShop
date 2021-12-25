#include "headers/registration.h"
#include "ui_registration.h"
#include "headers/mainwindow.h"
#include <QMessageBox>
#include <sstream>

registration::registration(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::registration) {
    ui->setupUi(this);
}

registration::~registration() {
    delete ui;
}


void registration::clear_window() {
    ui->fname_line->clear();
    ui->sname_line->clear();
    ui->birth_line->setDate(ui->birth_line->minimumDate());
    ui->email_line->clear();
    ui->pass_message->clear();
    ui->login_label->clear();
    ui->name_message->clear();
    ui->mail_label->clear();
    ui->log_line->clear();
    ui->pass1->clear();
    ui->pass2->clear();
}


void registration::on_checkBox_2_stateChanged(int) {
    if (ui->checkBox_2->isChecked()) {
        ui->pass2->setEchoMode(QLineEdit::Normal);
    } else {
        ui->pass2->setEchoMode(QLineEdit::Password);
    }
}


void registration::on_checkBox1_stateChanged(int) {
    if (ui->checkBox1->isChecked()) {
        ui->pass1->setEchoMode(QLineEdit::Normal);
    } else {
        ui->pass1->setEchoMode(QLineEdit::Password);
    }
}


//кнопка зарегистрироваться
void registration::on_pushButton_3_clicked() {
    ui->login_label->clear();
    ui->pass_message->clear();
    ui->name_message->clear();
    ui->mail_label->clear();

    checker ch;

    QString fname = QString(
            QCryptographicHash::hash(ui->fname_line->text().toUtf8(), QCryptographicHash::Sha256).toHex());
    QString sname = QString(
            QCryptographicHash::hash(ui->sname_line->text().toUtf8(), QCryptographicHash::Sha256).toHex());
    QString birth = ui->birth_line->text();
    QString email = ui->email_line->text();
    QString login = ui->log_line->text();
    QString password1 = QString(
            QCryptographicHash::hash(ui->pass1->text().toUtf8(), QCryptographicHash::Sha256).toHex());
    QString password2 = QString(
            QCryptographicHash::hash(ui->pass2->text().toUtf8(), QCryptographicHash::Sha256).toHex());

    if (!login.isEmpty() && !password1.isEmpty() && !password2.isEmpty() && !fname.isEmpty() && !sname.isEmpty() &&
        !email.isEmpty()) {

        if (!ch.Name_check(ui->fname_line->text()) || !ch.Name_check(ui->sname_line->text())) {
            ui->name_message->setText("Incorrect Fname or Sname");
            return;
        }
        if (!ch.Login_check(login)) {
            ui->login_label->setText("Incorrect Login");
            return;
        }
        if (password1 != password2) {
            ui->pass_message->setText("Passwords are not the same");
            return;
        }
        if (!ch.Email_check(email)) {
            ui->mail_label->setText("Incorrect email adress");
            return;
        }
        if (!ch.Pass_check(ui->pass1->text()) || !ch.Pass_check(ui->pass2->text())) {
            ui->pass_message->setText("Incorrect password format");
            return;
        }

        if (DataBase::person_exist(login)) {
            ui->login_label->setText("This Login is already taken");
            return;
        }

        email = QString(QCryptographicHash::hash(email.toUtf8(), QCryptographicHash::Sha256).toHex());
        std::thread th([login, password1, fname, sname, birth, email]() {
            DataBase::Get_db().connectToDataBase();
            DataBase::inserIntoTable(login, password1, fname, sname, birth, email);
        });
        th.detach();
        QMessageBox::StandardButton button = QMessageBox::information(this, "Registration",
                                                                      "You have successfully registered",
                                                                      QMessageBox::Ok);
        if (button == QMessageBox::Ok) {
            close();
            clear_window();
            emit firstWindow();
            emit lost_connection();
        }
    } else {
        QMessageBox::information(this, "Attention", "Please fill in all empty lines");
    }
}

//кнопка назад
void registration::on_pushButton_clicked() {
    this->close();
    clear_window();
    emit firstWindow();
}



