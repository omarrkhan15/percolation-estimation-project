#pragma once

#include <chrono>
#include <ratio>

// number of seconds per tick of the high-resolution clock
const double PERIOD = (double) std::chrono::high_resolution_clock::period::num / std::chrono::high_resolution_clock::period::den;

class Stopwatch {
public:
    Stopwatch() : start {std::chrono::high_resolution_clock::now()} {
    }
    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }
    double elapsedTime() const {
        const auto now = std::chrono::high_resolution_clock::now();
        return ((now - start).count()) * PERIOD; // convert to seconds and return
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start; 
};