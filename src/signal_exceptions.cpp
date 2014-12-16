#include "signal_exceptions.hpp"

no_such_signal::no_such_signal(const std::string& message) : _message(message) {
}

const char* no_such_signal::what() const throw() {
    return _message.c_str();
}
