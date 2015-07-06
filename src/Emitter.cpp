#include <Emitter.hpp>
#include <Delegate.hpp>
#include <stdlib.h>

Emitter::Emitter()
{
    this->signals            = (Signal*)calloc(MAX_SIGNALS, sizeof(signals));
    this->noActionSignal     = new Signal(nothing, &Nothing::noAction);
    this->nothing            = new Nothing();
    this->connectedDelegates = new std::list<Delegate*>();

    for (int i = 0; i < MAX_SIGNALS; i++)
    {
        this->signals[i] = *noActionSignal;
    }
}

Emitter::~Emitter()
{
    for(auto &delegate : *(this->connectedDelegates))
    {
        delegate->removeEmitter(this);
    }

    free(this->signals);
    delete(noActionSignal);
    delete(nothing);
    delete(connectedDelegates);
}

void Emitter::destroy(const int signalNumber)
{
    if (signalNumber > MAX_SIGNALS)
    {
        throw std::out_of_range("Bigger than signals number");
    }
    else if (signalNumber < 0)
    {
        throw std::out_of_range("Can't be smaller than signals zero");
    }
    this->signals[signalNumber] = *noActionSignal;
}

void Emitter::remove(Delegate* delegate)
{
    this->connectedDelegates->remove(delegate);
}
