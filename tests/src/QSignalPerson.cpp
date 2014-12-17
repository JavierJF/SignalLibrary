#include <QSignalPerson.hpp>

QtSignalPerson::QtSignalPerson(std::string name)
{
    this->name = name;
}

bool QtSignalPerson::compare(std::string name)
{
    if(this->name.compare(name) == 0) {
        return true;
    } else {
        return false;
    }
}

void QtSignalPerson::sendsignal(std::string* name) {
    emit print(name);
}
