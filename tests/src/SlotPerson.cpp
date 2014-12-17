#include <SlotPerson.hpp>
#include <iostream>

SlotPerson::SlotPerson(std::string name)
{
    this->name = name;
}

void SlotPerson::print(std::string name)
{
    std::cout << name << std::endl;
}

bool SlotPerson::compare(std::string* nmed)
{
    std::string as = *nmed;
    if(this->name.compare(as) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool SlotPerson::check(int a)
{
    if (a == 4)
        return true;
    return false;
}
