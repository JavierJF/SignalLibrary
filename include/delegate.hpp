/**
 * @file delegate.hpp File representing \c delegate class.
 *
 * @author Javier Jaramago Fernández
 *
 * @version 0.1
 */

#ifndef DELEGATE_HPP
#define DELEGATE_HPP

#include <map>
#include <vector>

using std::vector;
using std::map;

template <typename T>
vector<T> pushBack(vector<T> vector, T element)
{
    vector.push_back(element);
    return vector;
}

class Emitter;

class Delegate
{
    friend class Emitter;
private:
    map<Emitter*, vector<int>>* linkedEmitters;
protected:
    void addEmitter(Emitter* emitter, int signalNumber);
    void removeEmitter(Emitter* emitter);
public:
    Delegate();
    ~Delegate();
};

#endif
