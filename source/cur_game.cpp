#include "headers/cur_game.h"
#include "ui_cur_game.h"

cur_game::cur_game(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::cur_game) {
    ui->setupUi(this);
    ui->help_label->hide();
    ui->person->hide();
}

cur_game::~cur_game() {
    delete ui;
}

//заполнить страницу входными данными, полученными из окна Shop
void cur_game::set_game(const QString &login, const QString &gname) {
    LOGIN = login;
    QSqlQuery query;
    query.prepare("SELECT Price, Genre, Age, Date FROM Games WHERE Gname = :Name");
    query.bindValue(":Name", gname);
    if (query.exec()) {
        query.first();
        QString price = query.value(0).toString();
        QString genre = query.value(1).toString();
        QString age = query.value(2).toString();
        QString date = query.value(3).toString();
        ui->age_label->setText(age);
        ui->genre_label->setText(genre);
        ui->data_label->setText(date);
        ui->price_label->setText("$" + price);
        ui->gname_label->setText(gname);
    } else {
        qDebug() << "error";
    }
}


//кнопка купить
void cur_game::on_pushButton_clicked() {

    checker ch;
    QString price = ui->price_label->text();
    std::string pr = price.toStdString();
    pr = pr.substr(1, pr.size() - 1);

    price = QString::fromStdString(pr);
    QString gname = ui->gname_label->text();
    QString another_login = ui->person->text();


    // проверка на наличие игры в библиотеке

    if ((!ui->checkBox->isChecked() && DataBase::game_check(LOGIN, gname)) ||
        (ui->checkBox->isChecked() && LOGIN == another_login && DataBase::game_check(LOGIN, gname))) {
        QMessageBox::StandardButton button = QMessageBox::information(this, "Error of purchase",
                                                                      "You already have this game.",
                                                                      QMessageBox::StandardButton::Ok);
        if (button == QMessageBox::StandardButton::Ok) {
            return;
        }
    }

    QString age = ui->age_label->text();


    //покупка другу(проверки)
    if (ui->checkBox->isChecked()) {

        //если не введено ничего
        if (another_login.isEmpty()) {
            QMessageBox::StandardButton button = QMessageBox::warning(this, "Error of purchase",
                                                                      "You noted that you want to buy the game as a gift, \n"
                                                                      "but did not enter the recipient's login. Do it, or uncheck the box",
                                                                      QMessageBox::StandardButton::Ok);
            if (button == QMessageBox::StandardButton::Ok) {
                return;
            }
        }
        if (!ch.Login_check(another_login)) {
            QMessageBox::StandardButton button = QMessageBox::information(this, "Error of purchase",
                                                                          "Incorrect recipient's Login",
                                                                          QMessageBox::StandardButton::Ok);
            if (button == QMessageBox::StandardButton::Ok) {
                return;
            }
        }
        //cуществует ли такой пользователь
        if (!DataBase::person_exist(another_login)) {
            QMessageBox::StandardButton button = QMessageBox::warning(this, "Error of purchase",
                                                                      "There is no user with this login",
                                                                      QMessageBox::StandardButton::Ok);
            if (button == QMessageBox::StandardButton::Ok) {
                return;
            }
        }

        //если введенный пользователь уже имеет эту игру
        if (DataBase::game_check(another_login, gname)) {
            QMessageBox::StandardButton button = QMessageBox::information(this, "Error of purchase",
                                                                          "The person with the login " + another_login +
                                                                          " already has this game",
                                                                          QMessageBox::StandardButton::Ok);
            if (button == QMessageBox::StandardButton::Ok) {
                return;
            }
        }
        // если введенный пользователь не подходит по возрасту
        if (!ch.check_age_for_game(DataBase::get_BirthDate(another_login), age)) {
            QMessageBox::StandardButton button = QMessageBox::information(this, "Error of purchase",
                                                                          "The user " + another_login +
                                                                          " is not old enough for this game",
                                                                          QMessageBox::StandardButton::Ok);
            if (button == QMessageBox::StandardButton::Ok) {
                return;
            }
        }
        //проверка на наличие необходимой суммы
        if (DataBase::get_money(LOGIN).toDouble() - price.toDouble() >= 0) {

            QMessageBox::StandardButton button = QMessageBox::information(this, "Purchase",
                                                                          "Congratulations to you! The game is now in the " +
                                                                          another_login + "'s library",
                                                                          QMessageBox::StandardButton::Ok);
            if (button == QMessageBox::StandardButton::Ok) {
                std::thread th([&]() {
                    DataBase::Get_db().connectToDataBase();
                    DataBase::reduce_money(LOGIN, price, DataBase::get_money(LOGIN));
                    DataBase::inserIntoTable(another_login, gname);
                    emit refresh_money(LOGIN);

                });
                th.join();
                emit lost_connection();
                ui->person->clear();
                on_checkBox_stateChanged(0);
                close();
            }

        } else {
            QMessageBox::StandardButton button = QMessageBox::warning(this, "Error of purchase",
                                                                      "You don't have enough money",
                                                                      QMessageBox::StandardButton::Ok);
            if (button == QMessageBox::StandardButton::Ok) {
                return;
            }
        }
    } else {
        //покупка себе (проверки)
        if (!ch.check_age_for_game(DataBase::get_BirthDate(LOGIN), age)) {
            QMessageBox::StandardButton button = QMessageBox::information(this, "Error of purchase",
                                                                          "You are not old enough for this game",
                                                                          QMessageBox::StandardButton::Ok);
            if (button == QMessageBox::StandardButton::Ok) {
                return;
            }
        }
        if ((DataBase::get_money(LOGIN).toDouble() - price.toDouble()) >= 0) {
            QMessageBox::StandardButton button = QMessageBox::information(this, "Purchase",
                                                                          "Congratulations to you! The game is now in your library",
                                                                          QMessageBox::StandardButton::Ok);
            if (button == QMessageBox::StandardButton::Ok) {
                std::thread th([&]() {
                    DataBase::Get_db().connectToDataBase();
                    DataBase::reduce_money(LOGIN, price, DataBase::get_money(LOGIN));
                    DataBase::inserIntoTable(LOGIN, gname);
                    emit refresh_money(LOGIN);
                });

                th.join();
                emit lost_connection();
                ui->person->clear();
                close();
            }
        } else {
            QMessageBox::warning(this, "Error of purchase", "You don't have enough money");
        }
    }
}

//кнопка отмена
void cur_game::on_pushButton_2_clicked() {
    on_checkBox_stateChanged(0);
    close();
}


void cur_game::on_checkBox_stateChanged(int) {
    if (ui->checkBox->isChecked()) {
        ui->help_label->show();
        ui->person->show();
    } else {
        ui->help_label->hide();
        ui->person->clear();
        ui->person->hide();
    }
}



