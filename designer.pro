QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    source/checker.cpp \
    source/cur_game.cpp \
    source/database.cpp \
    source/main.cpp \
    source/mainwindow.cpp \
    source/money.cpp \
    source/pass_recovery.cpp \
    source/registration.cpp \
    source/shop.cpp \
    source/users_list.cpp

HEADERS += \
    headers/checker.h \
    headers/cur_game.h \
    headers/database.h \
    headers/mainwindow.h \
    headers/money.h \
    headers/pass_recovery.h \
    headers/registration.h \
    headers/shop.h \
    headers/users_list.h

FORMS += \
    graphic/cur_game.ui \
    graphic/mainwindow.ui \
    graphic/money.ui \
    graphic/pass_recovery.ui \
    graphic/registration.ui \
    graphic/shop.ui \
    graphic/users_list.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
