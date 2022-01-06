#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    QString gname = ui->lineEdit->text();
    QString price = ui->lineEdit_2->text();
    QString genre = ui->lineEdit_3->text();
    QString age = ui->lineEdit_4->text();
    QString date = ui->lineEdit_5->text();
    if(!gname.isEmpty() && !price.isEmpty() && !genre.isEmpty() && !age.isEmpty()){
        if(!check_price(price)){
            QMessageBox::StandardButton button = QMessageBox::information(this, "Incorrect price", "Please enter the correct price(only digits + '.' for the fractional part)",QMessageBox::StandardButton::Ok);
            if(button == QMessageBox::StandardButton::Ok){
                return;
            }
        }
        DataBase db;
        db.connectToDataBase();
        if(db.game_exist(gname)){
            QMessageBox::StandardButton button = QMessageBox::information(this, "Error", "There is already a game with this name",QMessageBox::StandardButton::Ok);
            if(button == QMessageBox::StandardButton::Ok){
                return;
            }

        }
        if(!check_age(age)){
            QMessageBox::StandardButton button = QMessageBox::information(this, "Error", "You have entered an incorrect age limit",QMessageBox::StandardButton::Ok);
            if(button == QMessageBox::StandardButton::Ok){
                return;
            }
        }
        db.inserIntoTable(gname,price,genre,age,date);
        lines_clear();
    }else{
        QMessageBox::StandardButton button = QMessageBox::information(this, "Attention", "Please fill in all the blank lines",QMessageBox::StandardButton::Ok);
        if(button == QMessageBox::StandardButton::Ok){
            return;
        }
    }
}


bool MainWindow::check_price(const QString &price)
{
    std::string str = price.toStdString();
    if(count(str.begin(),str.end(),'.')>1){
        return false;
    }
    for(const auto& sep : str){
        if((sep>='0' && sep <='9') || sep =='.'){
            continue;
        }else{
            return false;
        }
    }
    return true;
}

bool MainWindow::check_age(const QString &age)
{
    std::string str = age.toStdString();
    for(const auto& it:str){
        if(it<'0' || it>'9'){
            return false;
        }
    }
    int Age = age.toInt();
    if(Age > 21){
        return false;
    }
    return true;
}

void MainWindow::lines_clear()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->setDate(ui->lineEdit_5->minimumDate());
}

