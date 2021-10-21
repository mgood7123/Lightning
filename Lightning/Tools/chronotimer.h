#ifndef CHRONOTIMER_H
#define CHRONOTIMER_H

#include <QtGlobal>
#include <chrono>

class ChronoTimer {
public:
    class TimeUnit {
        quint64 nanosecondsTotal_;
        quint64 nanoseconds_;
        quint64 microsecondsTotal_;
        quint64 microseconds_;
        quint64 millisecondsTotal_;
        quint64 milliseconds_;
        quint64 secondsTotal_;
        quint64 seconds_;
        quint64 minutesTotal_;
        quint64 minutes_;
        quint64 hoursTotal_;
    public:
        class duration {
            public:
                typedef std::chrono::duration<quint64,         std::nano> nanoseconds;
                typedef std::chrono::duration<quint64,        std::micro> microseconds;
                typedef std::chrono::duration<quint64,        std::milli> milliseconds;
                typedef std::chrono::duration<quint64                   > seconds;
                typedef std::chrono::duration<quint64, std::ratio<  60> > minutes;
                typedef std::chrono::duration<quint64, std::ratio<3600> > hours;
            };
        TimeUnit();
        TimeUnit(const TimeUnit & timeUnit);;
        TimeUnit & operator=(const TimeUnit & timeUnit);;
        TimeUnit(const quint64 & nanoseconds);;
        TimeUnit & operator=(const quint64 & nanoseconds);;
        TimeUnit operator+(const TimeUnit & timeUnit);;
        TimeUnit operator-(const TimeUnit & timeUnit);;
        TimeUnit operator*(const TimeUnit & timeUnit);;
        TimeUnit operator/(const TimeUnit & timeUnit);;
        void set(const TimeUnit & timeUnit);
        void set(const quint64 & nanoseconds);
        quint64 nanosecondsTotal();
        quint64 nanoseconds();
        quint64 microsecondsTotal();
        quint64 microseconds();
        quint64 millisecondsTotal();
        quint64 milliseconds();
        quint64 secondsTotal();
        quint64 seconds();
        quint64 minutesTotal();
        quint64 minutes();
        quint64 hoursTotal();
    };
    TimeUnit startElapsedTime;
    TimeUnit currentElapsedTime;
    TimeUnit previousElapsedTime;
    TimeUnit difference;
    TimeUnit elapsed;
    bool started = false;

    static TimeUnit now();

    void reset();

    void elapsedTime();

    ChronoTimer();
    ChronoTimer(const ChronoTimer & chronoTimer);
    ChronoTimer & operator=(const ChronoTimer & chronoTimer);
};

#endif // CHRONOTIMER_H
