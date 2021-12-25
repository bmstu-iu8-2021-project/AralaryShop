#ifndef PASS_RECOVERY_H
#define PASS_RECOVERY_H

#include <QMainWindow>
#include "database.h"
#include "checker.h"
#include <thread>
#include <QThread>

namespace Ui {
    class pass_recovery;
}

class pass_recovery : public QMainWindow {
    Q_OBJECT

public:
    explicit pass_recovery(QWidget *parent = nullptr);

    ~pass_recovery();

    void en_lang();

    void ru_lang();

public
    slots:

            signals
    :

    void firstWindow();

    void lost_connetcion();

private
    slots:
            void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_checkBox_stateChanged(int);

    void on_checkBox_2_stateChanged(int);

    void clear_window();

private:
    Ui::pass_recovery *ui;
};

#endif // PASS_RECOVERY_H
