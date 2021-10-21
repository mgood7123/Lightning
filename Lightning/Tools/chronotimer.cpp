#include "chronotimer.h"

ChronoTimer::TimeUnit ChronoTimer::now() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
}

void ChronoTimer::reset() {
    startElapsedTime = now();
    previousElapsedTime = startElapsedTime - startElapsedTime;
    currentElapsedTime = startElapsedTime - startElapsedTime;
    started = true;
}

void ChronoTimer::elapsedTime() {
    previousElapsedTime = currentElapsedTime;
    currentElapsedTime = now() - startElapsedTime;
    elapsed = currentElapsedTime;
    difference = currentElapsedTime - previousElapsedTime;
}

ChronoTimer::ChronoTimer() {}

ChronoTimer::ChronoTimer(const ChronoTimer &chronoTimer) {
    startElapsedTime = chronoTimer.startElapsedTime;
    currentElapsedTime = chronoTimer.currentElapsedTime;
    previousElapsedTime = chronoTimer.previousElapsedTime;
    difference = chronoTimer.difference;
    elapsed = chronoTimer.elapsed;
}

ChronoTimer &ChronoTimer::operator=(const ChronoTimer &chronoTimer) {
    startElapsedTime = chronoTimer.startElapsedTime;
    currentElapsedTime = chronoTimer.currentElapsedTime;
    previousElapsedTime = chronoTimer.previousElapsedTime;
    difference = chronoTimer.difference;
    elapsed = chronoTimer.elapsed;
    return *this;
}

ChronoTimer::TimeUnit::TimeUnit() {
    nanosecondsTotal_ = 0;
    nanoseconds_ = 0;
    microsecondsTotal_ = 0;
    microseconds_ = 0;
    millisecondsTotal_ = 0;
    milliseconds_ = 0;
    secondsTotal_ = 0;
    seconds_ = 0;
    minutesTotal_ = 0;
    minutes_ = 0;
    hoursTotal_ = 0;
}

ChronoTimer::TimeUnit::TimeUnit(const TimeUnit &timeUnit) {
    set(timeUnit);
}

ChronoTimer::TimeUnit &ChronoTimer::TimeUnit::operator=(const TimeUnit &timeUnit) {
    set(timeUnit);
    return *this;
}

ChronoTimer::TimeUnit::TimeUnit(const quint64 &nanoseconds) {
    set(nanoseconds);
}

ChronoTimer::TimeUnit &ChronoTimer::TimeUnit::operator=(const quint64 &nanoseconds) {
    set(nanoseconds);
    return *this;
}

ChronoTimer::TimeUnit ChronoTimer::TimeUnit::operator+(const TimeUnit &timeUnit) {
    return TimeUnit(nanosecondsTotal_ + timeUnit.nanosecondsTotal_);
}

ChronoTimer::TimeUnit ChronoTimer::TimeUnit::operator-(const TimeUnit &timeUnit) {
    return TimeUnit(nanosecondsTotal_ - timeUnit.nanosecondsTotal_);
}

ChronoTimer::TimeUnit ChronoTimer::TimeUnit::operator*(const TimeUnit &timeUnit) {
    return TimeUnit(nanosecondsTotal_ * timeUnit.nanosecondsTotal_);
}

ChronoTimer::TimeUnit ChronoTimer::TimeUnit::operator/(const TimeUnit &timeUnit) {
    return TimeUnit(nanosecondsTotal_ / timeUnit.nanosecondsTotal_);
}

void ChronoTimer::TimeUnit::set(const TimeUnit &timeUnit) {
    nanosecondsTotal_ = timeUnit.nanosecondsTotal_;
    nanoseconds_ = timeUnit.nanoseconds_;
    microsecondsTotal_ = timeUnit.microsecondsTotal_;
    microseconds_ = timeUnit.microseconds_;
    millisecondsTotal_ = timeUnit.millisecondsTotal_;
    milliseconds_ = timeUnit.milliseconds_;
    secondsTotal_ = timeUnit.secondsTotal_;
    seconds_ = timeUnit.seconds_;
    minutesTotal_ = timeUnit.minutesTotal_;
    minutes_ = timeUnit.minutes_;
    hoursTotal_ = timeUnit.hoursTotal_;
}

void ChronoTimer::TimeUnit::set(const quint64 &nanoseconds) {
    duration::nanoseconds nanos(nanoseconds);
    nanosecondsTotal_ = nanoseconds;
    if (nanosecondsTotal_ == 0 || nanosecondsTotal_ < 1000) {
        nanoseconds_ = nanosecondsTotal_;
        microsecondsTotal_ = 0;
        microseconds_ = 0;
        millisecondsTotal_ = 0;
        milliseconds_ = 0;
        secondsTotal_ = 0;
        seconds_ = 0;
        minutesTotal_ = 0;
        minutes_ = 0;
        hoursTotal_ = 0;
    } else if (nanosecondsTotal_ == 1000) {
        nanoseconds_ = 0;
        microsecondsTotal_ = 1;
        microseconds_ = 1;
        millisecondsTotal_ = 0;
        milliseconds_ = 0;
        secondsTotal_ = 0;
        seconds_ = 0;
        minutesTotal_ = 0;
        minutes_ = 0;
        hoursTotal_ = 0;
    } else if (nanosecondsTotal_ > 1000) {
        duration::microseconds microseconds = std::chrono::duration_cast<duration::microseconds>(nanos);
        microsecondsTotal_ = microseconds.count();
        nanoseconds_ = (nanos - std::chrono::duration_cast<duration::nanoseconds>(microseconds)).count();
        if (microsecondsTotal_ < 1000) {
            microseconds_ = microsecondsTotal_;
            millisecondsTotal_ = 0;
            milliseconds_ = 0;
            secondsTotal_ = 0;
            seconds_ = 0;
            minutesTotal_ = 0;
            minutes_ = 0;
            hoursTotal_ = 0;
        } else if (microsecondsTotal_ == 1000) {
            microseconds_ = 0;
            milliseconds_ = 1;
            seconds_ = 0;
            minutesTotal_ = 0;
            minutes_ = 0;
            hoursTotal_ = 0;
        } else if (microsecondsTotal_ > 1000) {
            duration::milliseconds milliseconds = std::chrono::duration_cast<duration::milliseconds>(microseconds);
            millisecondsTotal_ = milliseconds.count();
            microseconds_ = (microseconds - std::chrono::duration_cast<duration::microseconds>(milliseconds)).count();
            if (millisecondsTotal_ < 1000) {
                milliseconds_ = millisecondsTotal_;
                secondsTotal_ = 0;
                seconds_ = 0;
                minutesTotal_ = 0;
                minutes_ = 0;
                hoursTotal_ = 0;
            } else if (millisecondsTotal_ == 1000) {
                milliseconds_ = 0;
                secondsTotal_ = 1;
                seconds_ = 1;
                minutesTotal_ = 0;
                minutes_ = 0;
                hoursTotal_ = 0;
            } else if (millisecondsTotal_ > 1000) {
                duration::seconds seconds = std::chrono::duration_cast<duration::seconds>(milliseconds);
                secondsTotal_ = seconds.count();
                milliseconds_ = (milliseconds - std::chrono::duration_cast<duration::milliseconds>(seconds)).count();
                if (secondsTotal_ < 60) {
                    seconds_ = secondsTotal_;
                    minutesTotal_ = 0;
                    minutes_ = 0;
                    hoursTotal_ = 0;
                } else if (secondsTotal_ == 60) {
                    seconds_ = 0;
                    minutesTotal_ = 1;
                    minutes_ = 1;
                    hoursTotal_ = 0;
                } else if (secondsTotal_ > 60) {
                    duration::minutes minutes = std::chrono::duration_cast<duration::minutes>(seconds);
                    minutesTotal_ = minutes.count();
                    seconds_ = (seconds - std::chrono::duration_cast<duration::seconds>(minutes)).count();
                    if (minutesTotal_ < 60) {
                        minutes_ = minutesTotal_;
                        hoursTotal_ = 1;
                    } else if (minutesTotal_ == 60) {
                        minutes_ = 0;
                        hoursTotal_ = 1;
                    } else if (minutesTotal_ > 60) {
                        duration::hours hours = std::chrono::duration_cast<duration::hours>(minutes);
                        hoursTotal_ = hours.count();
                        minutes_ = (minutes - std::chrono::duration_cast<duration::minutes>(hours)).count();
                    }
                }
            }
        }
    }
}

quint64 ChronoTimer::TimeUnit::nanosecondsTotal() {
    return nanosecondsTotal_;
}

quint64 ChronoTimer::TimeUnit::nanoseconds() {
    return nanoseconds_;
}

quint64 ChronoTimer::TimeUnit::microsecondsTotal() {
    return microsecondsTotal_;
}

quint64 ChronoTimer::TimeUnit::microseconds() {
    return microseconds_;
}

quint64 ChronoTimer::TimeUnit::millisecondsTotal() {
    return millisecondsTotal_;
}

quint64 ChronoTimer::TimeUnit::milliseconds() {
    return milliseconds_;
}

quint64 ChronoTimer::TimeUnit::secondsTotal() {
    return secondsTotal_;
}

quint64 ChronoTimer::TimeUnit::seconds() {
    return seconds_;
}

quint64 ChronoTimer::TimeUnit::minutesTotal() {
    return minutesTotal_;
}

quint64 ChronoTimer::TimeUnit::minutes() {
    return minutes_;
}

quint64 ChronoTimer::TimeUnit::hoursTotal() {
    return hoursTotal_;
}
