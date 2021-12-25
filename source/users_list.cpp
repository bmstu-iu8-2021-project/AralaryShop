#include "headers/users_list.h"
#include "ui_users_list.h"

users_list::users_list(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::users_list) {
    ui->setupUi(this);
}

users_list::~users_list() {
    delete ui;
}

//слот чтобы обновить список пользователей
void users_list::refresh_list(const QString &login) {
    QSqlQuery query;
    query.prepare("SELECT Login FROM Users WHERE Login <> :log");
    query.bindValue(":log", login);
    query.exec();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(std::move(query));
    ui->listView->setModel(model);
    ui->listView->show();
}

