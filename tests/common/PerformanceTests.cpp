#include <SlotPerson.hpp>
#include <EmitterPerson.hpp>
#include <QSignalPerson.hpp>
#include <QSlotPerson.hpp>
#include <gtest/gtest.h>
#include <string>
#include <functional>
// For timing
#include <chrono>
#include <QObject>
#include <iostream>

typedef std::chrono::high_resolution_clock hrc;

class PlainPerson {
private:
    SlotPerson* person;
    std::string name;
public:
    PlainPerson(SlotPerson* per) : person(per) {
        name = std::string("Name");
    }

    inline SlotPerson* get_Person() {
        return person;
    }

    inline void comparation() {
        this->person->compare(&name);
    }
};

TEST(TestSignalAssembly, shouldCreateSignal)
{
    std::string* name = new std::string("TestingName");
    SlotPerson* person = new SlotPerson("Name");

    int n = 10000000;

    hrc::time_point start;
    hrc::time_point end;
    std::chrono::nanoseconds duration;
    std::chrono::nanoseconds qt_duration;

    std::cout << "      " << std::endl;
    std::cout << "START " << std::endl;
    std::cout << "      " << std::endl;

    QtSignalPerson qtSignalPerson("A");
    QtSlotPerson qtSlotPerson("B");
    QObject::connect(&qtSignalPerson, SIGNAL(print(std::string*)),
                     &qtSlotPerson, SLOT(compare(std::string*)));

    //==========================
    //          START
    //==========================

    start = hrc::now();

    for(int i = 0; i < n; i++) {
        qtSignalPerson.sendsignal(name);
    }

    end = hrc::now();

    qt_duration = end - start;

    std::cout.width(35);
    std::cout << std::left << "Qt signal time:" <<
              std::setw(10) << std::right << qt_duration.count() << std::endl;

    //==========================
    //          END
    //==========================

    //==========================
    //          START
    //==========================

    //    std::function<void(std::string*)> f = std::bind(&SlotPerson::compare, person, std::placeholders::_1);
    //    Signal sigf(f);
    //
    //    start = hrc::now();
    //
    //    for(int i = 0; i < n; i++) {
    //        sigf.emiti(name);
    //    }
    //
    //    end = hrc::now();
    //
    //    duration = end - start;
    //
    //    std::cout.width(35);
    //    std::cout << std::left << "Standard based time:" <<
    //              std::setw(10) << std::right << duration.count() <<
    //              std::setw(16) << std::right << "Improvement: " << std::setw(8) << std::left <<
    //              (qt_duration.count()*100)/duration.count() << "%" << std::endl;

    //==========================
    //          END
    //==========================


    Signal sigz;
    sigz = Signal(person, &SlotPerson::compare, name);

    //==========================
    //          START
    //==========================

    start = hrc::now();

    for(int i = 0; i < n; i++) {
        sigz.launch();
    }

    end = hrc::now();

    duration = end - start;

    std::cout.width(35);
    std::cout << std::left << "Special signal time:" <<
              std::setw(10) << std::right << duration.count() <<
              std::setw(16) << std::right << "Improvement: " << std::setw(8) << std::left <<
              (qt_duration.count()*100)/duration.count() << "%" << std::endl;

    //==========================
    //          END
    //==========================

    EmitterPerson emitterPerson;

    _CONNECT_(emitterPerson, ACTIVATE, person, &SlotPerson::compare, name)

    //==========================
    //          START
    //==========================

    start = hrc::now();

    for(int i = 0; i < n; i++) {
        emitterPerson.launchSignal();
    }

    end = hrc::now();

    duration = end - start;

    std::cout.width(35);
    std::cout << std::left << "Emitted signal time:" <<
              std::setw(10) << std::right << duration.count() <<
              std::setw(16) << std::right << "Improvement: " << std::setw(8) << std::left <<
              (qt_duration.count()*100)/duration.count() << "%" << std::endl;

    //==========================
    //          END
    //==========================

    //==========================
    //          START
    //==========================

    start = hrc::now();

    for(int i = 0; i < n; i++) {
        person->compare(name);
    }

    end = hrc::now();

    duration = end - start;

    std::cout.width(35);
    std::cout << std::left << "Plain function signal time:" <<
              std::setw(10) << std::right << duration.count() <<
              std::setw(16) << std::right << "Improvement: " << std::setw(8) << std::left <<
              (qt_duration.count()*100)/duration.count() << "%" << std::endl;

    //==========================
    //          END
    //==========================

    //==========================
    //          START
    //==========================

    PlainPerson* plainPerson = new PlainPerson(person);

    start = hrc::now();

    for(int i = 0; i < n; i++) {
        plainPerson->comparation();
    }

    end = hrc::now();

    duration = end - start;

    std::cout.width(35);
    std::cout << std::left << "Direct call to variable time:" <<
              std::setw(10) << std::right << duration.count() <<
              std::setw(16) << std::right << "Improvement: " << std::setw(8) << std::left <<
              (qt_duration.count()*100)/duration.count() << "%" << std::endl;

    //==========================
    //          END
    //==========================

    std::cout << "   " << std::endl;
    std::cout << "END" << std::endl;
    std::cout << "   " << std::endl;

    ASSERT_TRUE(true);

    delete(person);
    delete(name);
    delete(plainPerson);
}
