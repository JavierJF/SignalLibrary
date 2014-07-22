#ifndef SIGNAL_H
#define SIGNAL_H

#include <functional>

template<class T>
using decayedtype = typename std::decay<T>::type;

class signal
{
private:

    class placeholder
    {
        public:
        virtual ~placeholder() {}

        virtual placeholder* clone() const = 0;
    };

    template<typename T>
    class derived : public placeholder
    {
        public:
        template<typename U> derived(U&& value) : value(std::forward<U>(value)) { }

        T value;

        placeholder* clone() const { return new derived<T>(value); }
    };

    placeholder* clone() const
    {
        if (ptr)
            return ptr->clone();
        else
            return nullptr;
    }

    placeholder* ptr;

    public:

    bool is_null() const { return !ptr; }
    bool not_null() const { return ptr; }

    template<typename U> signal(std::function<U> value)
        : ptr(new derived<decayedtype<std::function<U>>>(std::forward<std::function<U>>(value)))
    {

    }

    template<class U> bool is() const
    {
        typedef decayedtype<U> T;

        auto _derived = dynamic_cast<derived<T>*> (ptr);

        return _derived;
    }

    template<class U>
    decayedtype<U>& as()
    {
        typedef decayedtype<U> T;

        auto _derived = dynamic_cast<derived<T>*> (ptr);

        if (!_derived)
            throw std::bad_cast();

        return _derived->value;
    }

    template <typename... args>
    void emit(args... A)
    {
        auto _derived = dynamic_cast<derived<std::function<void(decltype(A)...)>>*> (ptr);

        if (!_derived)
            throw std::bad_cast();

        _derived->value(A...);
    }


    template<class U>
    operator U()
    {
        return as<decayedtype<U>>();
    }

    signal()
        : ptr(nullptr)
    {

    }

    signal(signal& that)
        : ptr(that.clone())
    {

    }

    signal(signal&& that)
        : ptr(that.ptr)
    {
        that.ptr = nullptr;
    }

    signal(const signal& that)
        : ptr(that.clone())
    {

    }

    signal(const signal&& that)
        : ptr(that.clone())
    {

    }

    signal& operator=(const signal& a)
    {
        if (ptr == a.ptr)
            return *this;

        auto old_ptr = ptr;

        ptr = a.clone();

        if (old_ptr)
            delete old_ptr;

        return *this;
    }

    signal& operator=(signal&& a)
    {
        if (ptr == a.ptr)
            return *this;

        std::swap(ptr, a.ptr);

        return *this;
    }

    ~signal()
    {
        if (ptr)
            delete ptr;
    }
};

#endif