#include "headers/mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include <QGroupBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
