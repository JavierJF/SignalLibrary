#ifndef QSIGNALPERSON_HPP
#define QSIGNALPERSON_HPP

#include <QObject>
#include <string>

class QtSignalPerson : public QObject {
Q_OBJECT
    std::string name;
public:
    QtSignalPerson(std::string name);
    bool compare(std::string name);
    void sendsignal(std::string* name);
signals:
    void print(std::string* name);
};

#endif // QSIGNALPERSON_HPP
