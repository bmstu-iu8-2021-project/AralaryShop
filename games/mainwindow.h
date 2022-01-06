#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <database.h>
#include <sstream>
#include <QMessageBox>
#include <QString>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
private:
    bool check_price(const QString &price);
    bool check_age(const QString& age);
    void lines_clear();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
