#pragma once
#ifndef CHECKER_H
#define CHECKER_H

#include <QString>
#include <regex>

class checker {
public:
    checker();

    bool Name_check(const QString &name);

    bool Email_check(const QString &email);

    bool Login_check(const QString &login);

    bool Pass_check(const QString &pass);

    bool money_check(const QString &str);

    bool phone_check(const QString &str);

    bool check_age_for_game(const QString &date, const QString &min_age);
};

#endif // CHECKER_H
