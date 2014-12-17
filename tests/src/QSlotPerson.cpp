#include <QSlotPerson.hpp>

QtSlotPerson::QtSlotPerson(std::string name) {
   this->name = name;
}

bool QtSlotPerson::compare(std::string* name) {
   std::string as = *name;
   if(this->name.compare(as) == 0) {
       return true;
   } else {
       return false;
   }
}
