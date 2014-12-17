#ifndef EMITTERPERSON_HPP
#define EMITTERPERSON_HPP

#include <Emitter.hpp>

class EmitterPerson : public Emitter {
public:
    _SIGNALS_
    (
        ACTIVATE
    )

    void launchSignal()
    {
        _EMIT_(ACTIVATE)
    }
};

#endif // EMITTERPERSON_HPP
