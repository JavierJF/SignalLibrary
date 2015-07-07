/**
 * @file Signal.hpp File representing \c Signal class.
 *
 * @author Javier Jaramago Fernández
 *
 * @version 0.1
 */

#ifndef SSIGNAL_HPP
#define SSIGNAL_HPP

#include <assert.h>
#include <memory>
#include <cxxabi.h>
#include <iostream>

template<class T> using decayedType = typename std::decay<T>::type;

/**
 * Helper types for parameter unpack.
 */
template<typename T>
struct is_pointer
{
    static const bool value = false;
};

template<typename T>
struct is_pointer<T*>
{
    static const bool value = true;
};

template<unsigned... Is>
struct indices
{
};

template<unsigned M, unsigned... Is>
struct indices_gen : indices_gen<M - 1, M - 1, Is...>
{
};

template<unsigned... Is>
struct indices_gen<0, Is...> : indices<Is...>
{
};

template <typename F, size_t num_args, typename... arguments>
struct unpack_caller
{
using indicesFor = indices_gen<num_args>;
private:
    template <typename Type, typename FuncType, unsigned... I>
    constexpr inline void call(Type T,
                               const FuncType &f,
                               const std::tuple<arguments...> &args,
                               indices<I...>) const
    {
        call_fun(T, f, args, indices<I...>());
    }

    template <typename Type, typename FuncType, unsigned... I>
    constexpr inline void call_fun(Type* const T,
                                   const FuncType &f,
                                   const std::tuple<arguments...> &args,
                                   indices<I...>) const
    {
        (T->*f)(std::get<I>(args)...);
    }

    template <typename Type, typename FuncType, unsigned... I>
    constexpr inline void call_fun(Type T,
                                   const FuncType &f,
                                   const std::tuple<arguments...> &args,
                                   indices<I...>) const
    {
        (T.*f)(std::get<I>(args)...);
    }

    template <typename FuncType, unsigned... I>
    constexpr inline void call(const FuncType &f,
                               const std::tuple<arguments...> &args,
                               indices<I...>) const
    {
        call_fun(f, args, indices<I...>());
    }

    template <typename FuncType, unsigned... I>
    constexpr inline void call_fun(const FuncType &f,
                                   const std::tuple<arguments...> &args,
                                   indices<I...>) const
    {
        (*f)(std::get<I>(args)...);
    }

    template <typename Type, typename FuncType, unsigned... I>
    constexpr inline void call_fun(const FuncType &f,
                                   const std::tuple<arguments...> &args,
                                   indices<I...>) const
    {
        (*f)(std::get<I>(args)...);
    }

public:
    template <typename Type, typename FuncType>
    constexpr inline void operator() (Type T,
                                      const FuncType &f,
                                      const std::tuple<arguments...> &args) const
    {
        static_assert(sizeof...(arguments) == num_args, "Equal number of arguments");
        call(T, f, args, indicesFor {});
    }

    template <typename FuncType>
    constexpr inline void operator () (const FuncType &f,
                                       const std::tuple<arguments...> &args) const
    {
        static_assert(sizeof...(arguments) == num_args,"Equal number of arguments");
        call(f, args, indicesFor {});
    }
};

/*!
 * Base class for type-erasure mechanism.
 */
class placeholder
{
public:
    virtual ~placeholder() {}
    virtual const placeholder* clone() const = 0;
    virtual void execute() const = 0;
};

/**
 * This class represent through its base class the storing of the signal for
 * its execution.
 *
 * Explanation:
 *     This is a templated class, which means that each specialization is a
 *     different type, so we cannot store it in the same type, unless this type
 *     is also templated. That makes impossible to make a truly generic Signal,
 *     unless you use a type-erasure technique, exposing a common interface,
 *     for all these specific templated classes.
 */
template <typename T, typename F, typename... arguments>
class derived final : public placeholder {
private:
    T* const type;
    const F funct;

    const std::tuple<arguments...> argum;
    const unpack_caller<F, sizeof...(arguments), arguments...> up;

public:
    derived(T* const type, const F funct, const arguments... args) :
        type(type), funct(funct), argum(std::make_tuple(args...)),
        up(unpack_caller<F, sizeof...(arguments), arguments...>())
    {
    }

    derived(T* const type, const F funct,
            const std::tuple<arguments...> argum) : type(type),
                                                    funct(funct),
                                                    argum(argum),
                                                    up(unpack_caller<F, sizeof...(arguments), arguments...>())
    {
    }

    derived(derived<T,F,arguments...>&& other) : funct(other.funct),
                                                 type(other.type),
                                                 argum(std::move(other.argum)),
                                                 up(std::move(other.up))
    {
        other.type = NULL;
    }

    inline void execute() const override final
    {
        up(this->type, this->funct, this->argum);
    }

    const placeholder* clone() const final
    {
        return new derived<decayedType<T>, decayedType<F>,
            decayedType<arguments>...>(this->type, this->funct, this->argum);
    }

    placeholder& operator=(derived<T,F,arguments...> other)
    {
        if(this!=other)
        {
            this->funct = other.funct;
            this->type = other.type;
            this->argum = other.argum;
            this->up = other.up;
            other->type = NULL;
        }
        return *this;
    }
};

/*!
 * The purpose of this class is to act like a generic container that
 * makes possible to store any kind of function. This is achieve through
 * the same techniques of \c Boost::Any.
 *
 * @todo Class isn't in a production ready state. Assembly generated hasn't
 *       been analyzed yet, so the type erasure mechanism hasn't been optimized.
 *       This could generate a significant performance impact due right now the
 *       compiler is going to be force to make indirect calls.
 */
class Signal
{
private:
    /*!
     * @brief Internal pointer used for type-erasure.
     */
    const placeholder* _ptr;

    /*!
     * Clone the internal type that the \c Signal holds.
     *
     * @return If exists, returns the internal \c placeholder pointer \c Signal::_ptr,
     *         otherwise returns a "null pointer".
     */
    const placeholder* clone() const
    {
        if (_ptr)
            return _ptr->clone();
        else
            return nullptr;
    }

public:
    /*!
     * Constructs a new \c Signal.
     *
     * @param value Function that will be stored in the \c Signal.
     *
     * @tparam U Type of the \c std::function that is going to be stored.
     */
    template <typename Type, typename F, typename... args>
    Signal(Type* const type,
            F&& func,
            args&&... arguments) : _ptr(new derived<decayedType<Type>,
                                        decayedType<F>,
                                        decayedType<args>...>(std::forward<Type* const>(type),
                                        std::forward<F>(func),
                                        std::forward<args>(arguments)...))
    {
    }

    /*!
     * @brief Constructs an empty \c Signal.
     */
    Signal()
        : _ptr()
    {
    }

    /*!
     * @brief Move constructor.
     *
     * @param sig Rvalue reference from which the \c Signal::_ptr will be taken.
     */
    Signal(Signal&& sig)
        : _ptr(sig._ptr)
    {
    }

    /*!
     * @brief Copy constructor.
     *
     * @param sig Reference whose pointed type is going to be copied.
     */
    Signal(const Signal& sig)
        : _ptr(sig.clone())
    {
    }

    /*!
     * @brief Move constructor.
     *
     * @param sig Rvalue reference whose pointed type is going to be copied.
     *
     * @todo This method takes a \c const rvalue reference, that make impossible to
     *       stole its resources, which is the main purpose of the move constructors.
     *       It will be necessary to evaluate the real advantages of having this function.
     */
    Signal(const Signal&& sig)
        : _ptr(sig.clone())
    {
    }

    /*!
     * @brief Destructor.
     *
     * @note Simply destroys the internal Signal::_ptr.
     */
    ~Signal()
    {
        if (_ptr)
            delete _ptr;
    }

    /*!
     * Checks if internal placeholder \c Signal::_ptr pointer is \c null.
     *
     * @return If \c Signal::_ptr is null returns \c TRUE, \c FALSE otherwise.
     */
    bool is_null() const
    {
        return !_ptr;
    }

    /*!
     * Checks if internal placeholder \c Signal::_ptr pointer isn't null.
     *
     * @return If \c Signal::_ptr isn't null returns \c TRUE, \c FALSE otherwise.
     */
    bool not_null() const
    {
        return _ptr;
    }

    std::shared_ptr<char> demangle(const char* name)
    {
        int status;
        auto ptr = abi::__cxa_demangle(name, 0, 0, &status);
        if (ptr != NULL) {
            return std::shared_ptr<char>(ptr, std::free);
        }
        std::cerr << "couldn't demangle: " << status << std::endl;
        return std::shared_ptr<char>();
    }

    /*!
     * @brief Executes the function that holds \c Signal::_ptr, represent the
     *        functional core of the class.
     *
     * @tparam args The types of the parameters that will be the function
     *              arguments.
     *
     * @param A The function parameters that will be passed to the \c Signal::_ptr
     *          pointer.
     *
     * @throws std::bad_cast In case the current \c Signal::_ptr can't be casted to the desired function type.
     *         @details
     *         This could mean several things:
     *
     *             - The function held doesn't have the same type than the passed one. That
     *               could be due an error in the parameters order, or similar writing mistakes.
     *
     *             - Then passed type isn't a std::function.
     */
    inline void launch() const
    {
        this->_ptr->execute();
    }

    /*!
     * @brief Assignment operator.
     *
     * @param sig \c Signal that is going to be assigned.
     */
    Signal& operator=(const Signal& sig)
    {
        if (_ptr == sig._ptr)
            return *this;

        auto old__ptr = _ptr;

        _ptr = sig.clone();

        if (old__ptr)
            delete old__ptr;

        return *this;
    }

    /*!
     * @brief Move assignment operator.
     *
     * @param sig Rvalue reference \c Signal whose resources are going to be stoled.
     */
    Signal& operator=(Signal&& sig)
    {
        if (_ptr == sig._ptr)
            return *this;

        std::swap(_ptr, sig._ptr);

        return *this;
    }
};

#endif  // SIGNAL_HPP
