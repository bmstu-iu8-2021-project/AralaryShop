#include "headers/shop.h"
#include "ui_shop.h"

Shop::Shop(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::Shop) {
    ui->setupUi(this);
    ui->vkladka_label->clear();
    ui->listView->hide();
    ui->pushButton_6->hide();
    ui->pushButton_7->hide();
    m_window = new money(this);

    connect(m_window, &money::firstWindow, this, &Shop::show);
    connect(this, &Shop::sent_login, m_window, &money::set_person);
    connect(m_window, &money::refresh, this, &Shop::set_person);
    connect(m_window, &money::lost_connection, this, &Shop::refreshrer);
    connect(m_window, &money::send_status, this, &Shop::set_status);

    temp_game = new cur_game(this);
    connect(temp_game, &cur_game::refresh_money, this, &Shop::set_person);
    connect(this, &Shop::sent_game, temp_game, &cur_game::set_game);
    connect(temp_game, &cur_game::lost_connection, this, &Shop::refreshrer);

    ulist = new users_list(this);
    connect(this, &Shop::get_users_list, ulist, &users_list::refresh_list);
}

Shop::~Shop() {

    delete temp_game;
    delete m_window;
    delete ulist;
    delete model;
    delete ui;
}

//кнопка logout
void Shop::on_pushButton_clicked() {
    this->close();
    ui->statusbar->clearMessage();

    ulist->close();
    temp_game->close();
    m_window->close();
    emit firstWindow();
}

//слот для установки данных пользователя после входа
void Shop::set_person(const QString &login) {
    LOGIN = login;
    ui->login_label->setText(login);
    ui->cash_label->setText("$" + DataBase::get_money(login));
    on_pushButton_3_clicked();
}


void Shop::set_status() {
    ui->statusbar->showMessage("You have successfully replenished your balance");
}

void Shop::refreshrer() {
    DataBase::Get_db().connectToDataBase();
}


//кнопка пополнить баланс
void Shop::on_pushButton_4_clicked() {
    std::thread th([this]() {
        QString money = ui->cash_label->text();
        std::string cash = money.toStdString();
        cash = cash.substr(1, cash.size() - 1);
        money = QString::fromStdString(cash);
        emit sent_login(LOGIN, money);
    });
    th.detach();
    m_window->show();
}


//нажатие на кнопку магазин
void Shop::on_pushButton_2_clicked() {
    ui->vkladka_label->setText("Game store:");
    ui->tableView->show();
    ui->listView->hide();
    ui->pushButton_6->hide();
    ui->pushButton_7->hide();

    model = new QSqlQueryModel();
    model->setQuery("SELECT Gname, Price FROM Games ");
    model->setHeaderData(0, Qt::Horizontal, "Name of the game");
    model->setHeaderData(1, Qt::Horizontal, "Price $");
    ui->tableView->setModel(model);

    //столбец таблицы растягивается до конца
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    //при нажатии на ячейку выбирается строчка целиком
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //столбик 1 нельзя растягивать
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);

}


//двойное нажатие на таблицу игру(вызов окна с выбранной игрой)
void Shop::on_tableView_doubleClicked(const QModelIndex &index) {

    temp_game->hide();
    QString str;
    if (index.column() == 0) {
        str = (index.model()->data(index.model()->index(index.row(), index.column()))).toString();
    } else {
        str = (index.model()->data(index.model()->index(index.row(), index.column() - 1))).toString();
    }
    std::thread th([this, str]() {
        emit sent_game(LOGIN, str);
    });
    th.detach();
    temp_game->show();
}


//двойной клин на игру из вашей библиотеки
void Shop::on_listView_doubleClicked(const QModelIndex &index) {
    QString str = index.data(0).toString();

    if (ui->vkladka_label->text() == "Game library: your favourite games") {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Attention",
                                                                  "Are you sure you want to remove the game " + str +
                                                                  " from favorites?", QMessageBox::StandardButton::Yes |
                                                                                      QMessageBox::StandardButton::No);
        if (reply == QMessageBox::StandardButton::Yes) {

            DataBase::change_favourite(LOGIN, str);
        }
        on_pushButton_7_clicked();
    } else {
        if (DataBase::get_favourite(LOGIN, str)) {
            QMessageBox::information(this, "Attention", "Game " + str + " is already in your favorites list");
        } else {
            QMessageBox::StandardButton reply = QMessageBox::question(this, "Attention",
                                                                      "Do you want to add the game " + str +
                                                                      " to your favorites?",
                                                                      QMessageBox::StandardButton::Yes |
                                                                      QMessageBox::StandardButton::No);
            if (reply == QMessageBox::StandardButton::Yes) {
                DataBase::change_favourite(LOGIN, str);
            }
        }
    }
    return;
}

//конпка все ваши игры
void Shop::on_pushButton_6_clicked() {

    ui->vkladka_label->setText("Game library: all your games");
    model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT Gname FROM person_games WHERE Login = :log");
    query.bindValue(":log", LOGIN);
    query.exec();
    model->setQuery(std::move(query));

    ui->listView->setModel(model);
    ui->listView->show();
}

//кнопка библиотека
void Shop::on_pushButton_3_clicked() {
    on_pushButton_6_clicked();
    ui->tableView->hide();
    ui->listView->show();
    ui->pushButton_6->show();
    ui->pushButton_7->show();
}

//кнопка избранные игры
void Shop::on_pushButton_7_clicked() {
    ui->vkladka_label->setText("Game library: your favourite games");

    model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT Gname FROM person_games WHERE Login = :log AND IsFavourite = :flag");
    query.bindValue(":log", LOGIN);
    query.bindValue(":flag", true);
    query.exec();
    model->setQuery(std::move(query));

    ui->listView->setModel(model);
    ui->listView->show();
    return;
}

//кнопка все пользователи
void Shop::on_pushButton_5_clicked() {
    std::thread th([this]() {
        emit get_users_list(LOGIN);
    });
    th.detach();
    ulist->show();
}

