#ifndef SLOTPERSON_HPP
#define SLOTPERSON_HPP

#include <delegate.hpp>
#include <string>

class SlotPerson : public Delegate
{
    std::string name;
public:
    SlotPerson(std::string name);
    void print(std::string name);
    bool compare(std::string* nmed);
    bool check(int a);
};

#endif
