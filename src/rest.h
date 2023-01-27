#pragma once

#include <chrono>
#include <string>

class MyRestCall {
public:
    using duration_t = std::chrono::duration<int>;
    using time_point_t = std::chrono::time_point<std::chrono::system_clock>;

    MyRestCall() = delete;
    MyRestCall(MyRestCall&) = delete;
    MyRestCall(MyRestCall&&) = delete;

    MyRestCall(duration_t skipPeriod)
        : _skipPeriod{skipPeriod}
        , _nextFire{std::chrono::system_clock::now()}
    {}

    bool call(std::string motionState);
private:
    duration_t _skipPeriod;
    time_point_t _nextFire;
};