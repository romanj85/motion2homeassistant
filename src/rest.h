#pragma once

#include <chrono>
#include <string>

class MotionRestCall {
public:
    using duration_t = std::chrono::duration<int>;
    using time_point_t = std::chrono::time_point<std::chrono::system_clock>;

    MotionRestCall() = delete;
    MotionRestCall(MotionRestCall&) = delete;
    MotionRestCall(MotionRestCall&&) = delete;

    MotionRestCall(const std::string webhook, duration_t skipPeriod)
        : _webhook(webhook)
        , _skipPeriod{skipPeriod}
        , _nextFire{std::chrono::system_clock::now()}
    {}

    bool call(const std::string motionState);
    bool notify();
private:
    bool skipFastCall(const std::string motionState);
private:
    std::string _webhook;
    duration_t _skipPeriod;
    time_point_t _nextFire;
};