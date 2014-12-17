#ifndef SIGNALEXCEPTIONS__HPP
#define SIGNALEXCEPTIONS__HPP

#include <string>

class no_such_signal: public std::exception {
private:
    std::string _message;
public:
    no_such_signal(const std::string& message);
    virtual const char* what() const throw();
};

#endif // SIGNALEXCEPTIONS__H
