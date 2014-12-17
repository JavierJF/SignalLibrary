#ifndef SPERSON_HPP
#define SPERSON_HPP

#include <QObject>

class QtSlotPerson : public QObject {
Q_OBJECT
   std::string name;
public:
   QtSlotPerson(std::string name);
public slots:
   bool compare(std::string* name);
};

#endif // SPERSON_HPP
