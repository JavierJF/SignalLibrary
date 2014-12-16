#include <emitter.hpp>
#include <delegate.hpp>

Delegate::Delegate()
{
    this->linkedEmitters = new map<Emitter*, vector<int>>();
}

Delegate::~Delegate()
{
    for (auto &linkedEmitterPair : *(this->linkedEmitters))
    {
        for (auto signalNumber : linkedEmitterPair.second)
        {
            linkedEmitterPair.first->destroy(signalNumber);
        }
        linkedEmitterPair.first->remove(this);
    }
    delete(this->linkedEmitters);
}

void Delegate::addEmitter(Emitter* emitter, int signalNumber)
{
    bool found = false;

    for(auto emitterPair : *(this->linkedEmitters))
    {
        if (emitterPair.first == emitter)
        {
            emitterPair.second.push_back(signalNumber);
            found=true;
            break;
        }
    }

    if (!found)
    {
        vector<int> signalNumbers;
        signalNumbers.push_back(signalNumber);
        this->linkedEmitters->insert(make_pair(emitter, signalNumbers));
    }
}

void Delegate::removeEmitter(Emitter* emitter)
{
    this->linkedEmitters->erase(emitter);
}
