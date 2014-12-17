#ifndef EMITTER_HPP
#define EMITTER_HPP

#include <Signal.hpp>
#include <SignalExceptions.hpp>
#include <list>

#define _SIGNALS_(...)                                             			\
    enum SIGNALS {__VA_ARGS__};

#define _CONNECT_(Emitter, SignalName, Delegate, Fun, ...)                  \
    connect(decltype(Emitter)::SIGNALS::SignalName, Emitter, Delegate, Fun, \
            ##__VA_ARGS__);

#define _EMIT_(...)                                                         \
    this->emit<SIGNALS::__VA_ARGS__>();

#define MAX_SIGNALS 1000

class Nothing
{
public:
    void noAction()
    {
    }
};

class Delegate;

class Emitter
{
    friend class Delegate;
private:
    sep::Signal*            signals;
    sep::Signal*            noActionSignal;
    Nothing*                nothing;
    std::list<Delegate*>*   connectedDelegates;

    void destroy(const int signalName);
    void remove(Delegate* delegate);
protected:
    template<int num>
    constexpr void emit() const;
public:
    Emitter();
    ~Emitter();
    template <typename T, typename F, typename... args>
    void connect(const int signalName, T&& type, F&& f,
                 args... delegateFunctionArguments);
};

template<int signalNumber>
inline constexpr void Emitter::emit() const
{
    static_assert(signalNumber <= MAX_SIGNALS,
                  "Signal number can't be bigger than MAX_SIGNALS.");
    static_assert(signalNumber >= 0,
                  "Signal number can't be smaller than zero.");
    this->signals[signalNumber].launch();
}

// Necessary to have own template parameters in a function after that, to use
// template expansion for the indices.
template <typename T, typename F, typename... args>
void Emitter::connect(const int signalName, T&& delegate, F&& delegateFunction,
                      args... delegateFunctionArguments)
{
    if (signalName < 0 || signalName > MAX_SIGNALS)
    {
        throw std::out_of_range("Not in range signal name.");
    }

    delegate->addEmitter(this, signalName);
    this->connectedDelegates->push_back(delegate);
    this->signals[signalName] = sep::Signal(std::forward<T>(delegate),
                                            std::forward<F>(delegateFunction),
                                            std::forward<args>(delegateFunctionArguments)...);
}

template <typename T, typename F, typename... args>
void connect(const int signalName, Emitter& emitter, const T& delegate,
             const F& delegateFunction, args... delegateFunctionArguments)
{
    static_assert(std::is_base_of<Delegate,
                  std::remove_pointer_t<decayedType<T>>>::value, "Not a Delegate!.");
    emitter.connect(signalName, delegate, delegateFunction, delegateFunctionArguments...);
}

#endif // EMITTER_HPP
