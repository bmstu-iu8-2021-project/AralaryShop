#include <headers/checker.h>

checker::checker() {}

bool checker::Login_check(const QString &login) {
    for (const auto &it: login) {
        if ((it >= '0' && it <= '9') || (it >= 'A' && it <= 'z') || (it == '_')) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

bool checker::Pass_check(const QString &pass) {
    for (const auto &it: pass) {
        if (it == ' ') {
            return false;
        }
    }
    return true;
}

//проверка почты
bool checker::Email_check(const QString &email) {
    std::string mail = email.toStdString();
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return regex_match(mail, pattern);
}

//проверка имени или фамилии
bool checker::Name_check(const QString &name) {
    bool f1 = true;
    for (const auto &it: name) {
        if ((it >= 'A' && it <= 'z')) {
            continue;
        } else {
            f1 = false;
            break;
        }
    }
    bool f2 = true;
    std::string abc = "йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";
    std::string Name = name.toStdString();
    for (const auto &it: Name) {
        if (std::find(abc.begin(), abc.end(), it) == abc.end()) {
            f2 = false;
            break;
        }
    }
    return (f1 || f2);
}

//проверка денег
bool checker::money_check(const QString &str) {
    std::string temp = str.toStdString();
    if (temp.empty()) {
        return false;
    }
    if (std::count(temp.begin(), temp.end(), '.') > 1) {
        return false;
    }
    for (const auto &it: temp) {
        if (it < '0' || it > '9') {
            if (it == '.') {
                continue;
            }
            return false;
        }
    }
    return true;
}

//проверка телефона
bool checker::phone_check(const QString &str) {
    std::string temp = str.toStdString();
    if (temp.empty()) {
        return true;
    }
    if (temp.size() < 9 || temp.size() > 15 || temp[0] != '+') {
        return false;
    }
    for (size_t i = 1; i < temp.size(); ++i) {
        if (temp[i] < '0' || temp[i] > '9') {
            return false;
        }
    }
    return true;
}

//проверка возраста для покупки игры
bool checker::check_age_for_game(const QString &date, const QString &min_age) {
    std::string str = date.toStdString();
    str = str.substr(str.size() - 4, 4);
    std::istringstream stream(str);
    int year = 0;
    stream >> year;
    if (2021 - year >= min_age.toInt()) {
        return true;
    } else {
        return false;
    }
}
