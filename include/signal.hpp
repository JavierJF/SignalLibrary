/**
 * @file signal.hpp File representing \c signal class.
 *
 * @author Javier Jaramago Fernández
 *
 * @version 0.1
 */

#ifndef SIGNAL_H
#define SIGNAL_H

#include <functional>

template<class T>
using decayedtype = typename std::decay<T>::type;

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
class signal
{
private:

    /*!
     * Base class for type-erasure mechanism.
     */
    class placeholder
    {
    public:
        virtual ~placeholder() {}

        virtual placeholder* clone() const = 0;
    };

    /*!
     * Class that can hold any type.
     *
     * @tparam T Type of the object that is going to be stored.
     */
    template<typename T>
    class derived : public placeholder
    {
    public:
        template<typename U> derived(U&& value) : _value(std::forward<U>(value)) { }

        T _value;

        placeholder* clone() const {
            return new derived<T>(_value);
        }
    };

    /*!
     * Clone the internal type that the \c signal holds.
     *
     * @return If exists, returns the internal \c placeholder pointer \c signal::_ptr,
     *         otherwise returns a "null pointer".
     */
    placeholder* clone() const
    {
        if (_ptr)
            return _ptr->clone();
        else
            return nullptr;
    }

    /*!
     * @brief Internal pointer used for type-erasure.
     */
    placeholder* _ptr;

public:

    /*!
     * Constructs a new \c signal.
     *
     * @param value Function that will be stored in the \c signal.
     *
     * @tparam U Type of the \c std::function that is going to be stored.
     */
    template<typename U> signal(std::function<U> value)
        : _ptr(new derived<decayedtype<std::function<U>>>(std::forward<std::function<U>>(value)))
    {
    }

    /*!
     * @brief Constructs an empty \c signal.
     */
    signal()
        : _ptr(nullptr)
    {
    }

    /*!
     * @brief Non-Constant copy constructor a \c signal from a reference.
     *
     * @param sig Reference whose pointed type is going to be copied.
     *
     * @todo This method is a copy constructor with a non-constant argument.
     *       The necessity of this should be evaluated. It's unusual and probably
     *       meaningless.
     */
    signal(signal& sig)
        : _ptr(sig.clone())
    {
    }

    /*!
     * @brief Move constructor.
     *
     * @param sig Rvalue reference from which the \c signal::_ptr will be taken.
     */
    signal(signal&& sig)
        : _ptr(sig._ptr)
    {
        sig._ptr = nullptr;
    }

    /*!
     * @brief Copy constructor.
     *
     * @param sig Reference whose pointed type is going to be copied.
     */
    signal(const signal& sig)
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
    signal(const signal&& sig)
        : _ptr(sig.clone())
    {
    }

    /*!
     * @brief Destructor.
     *
     * @note Simply destroys the internal signal::_ptr.
     */
    ~signal()
    {
        if (_ptr)
            delete _ptr;
    }

    /*!
     * Checks if internal placeholder \c signal::_ptr pointer is \c null.
     *
     * @return If \c signal::_ptr is null returns \c TRUE, \c FALSE otherwise.
     */
    bool is_null() const {
        return !_ptr;
    }

    /*!
     * Checks if internal placeholder \c signal::_ptr pointer isn't null.
     *
     * @return If \c signal::_ptr isn't null returns \c TRUE, \c FALSE otherwise.
     */
    bool not_null() const {
        return _ptr;
    }

    /*!
     * @brief Checks if the held placeholder \c signal::_ptr is of the same type
     *        than a given one at runtime.
     *
     * @tparam U Type with which the comparison is going to be make.
     *
     * @throws std::bad_cast In case that the given type can't be cast to the held one.
     *
     * @retval TRUE If the given type match the contained one.
     *
     * @retval FALSE Otherwise.
     */
    template<class U> bool is() const
    {
        typedef decayedtype<U> T;

        auto deriv = dynamic_cast<derived<T>*> (_ptr);

        return deriv;
    }

    /*!
     * @brief Cast the held placeholder \c signal::_ptr to a given type at
     *        runtime.
     *
     * @tparam U Type in which the \c signal::_ptr is going to be casted.
     *
     * @throws std::bad_cast In case that the given type can't be cast to the
     *                       held one.
     *
     * @returns The \c signal::_ptr casted to the specified type.
     */
    template<class U>
    decayedtype<U>& as()
    {
        typedef decayedtype<U> T;

        auto _derived = dynamic_cast<derived<T>*> (_ptr);

        if (!_derived)
            throw std::bad_cast();

        return _derived->_value;
    }

    /*!
     * @brief Executes the function that holds \c signal::_ptr, represent the
     *        functional core of the class.
     *
     * @tparam args The types of the parameters that will be the function
     *              arguments.
     *
     * @param A The function parameters that will be passed to the \c signal::_ptr
     *          pointer.
     *
     * @throws std::bad_cast In case the current \c signal::_ptr can't be casted to the desired function type.
     *         @details
     *         This could mean several things:
     *
     *             - The function held doesn't have the same type than the passed one. That
     *               could be due an error in the parameters order, or similar writing mistakes.
     *
     *             - Then passed type isn't a std::function.
     */
    template <typename... args>
    void emit(args... A)
    {
        auto _derived = dynamic_cast<derived<std::function<void(decltype(A)...)>>*> (_ptr);

        if (!_derived)
            throw std::bad_cast();

        _derived->_value(A...);
    }

    /*!
     * @brief Conversion operator.
     *
     * @throws std::bad_cast In case the conversion can't be performed.
     */
    template<class U>
    operator U()
    {
        return as<decayedtype<U>>();
    }

    /*!
     * @brief Assignment operator.
     *
     * @param sig \c Signal that is going to be assigned.
     */
    signal& operator=(const signal& sig)
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
     * @param sig Rvalue reference \c signal whose resources are going to be stoled.
     */
    signal& operator=(signal&& sig)
    {
        if (_ptr == sig._ptr)
            return *this;

        std::swap(_ptr, sig._ptr);

        return *this;
    }
};

#endif
