#include "SignalStrategy.h"

FixedTimingStrategy::FixedTimingStrategy(int fixedGreenTime) {
    greenTime = fixedGreenTime;
}

std::pair<int, int> FixedTimingStrategy::getGreenTimes(int, int) const {
    return {greenTime, greenTime};
}

std::string FixedTimingStrategy::getName() const {
    return "Fixed Timing";
}

AdaptiveTimingStrategy::AdaptiveTimingStrategy(int base, int extra) {
    baseGreenTime = base;
    extraGreenTime = extra;
}

std::pair<int, int> AdaptiveTimingStrategy::getGreenTimes(int nsCount, int ewCount) const {
    int nsGreen = baseGreenTime;
    int ewGreen = baseGreenTime;

    if (nsCount > ewCount) {
        nsGreen += extraGreenTime;
    }
    else if (ewCount > nsCount) {
        ewGreen += extraGreenTime;
    }

    return {nsGreen, ewGreen};
}

std::string AdaptiveTimingStrategy::getName() const {
    return "Adaptive Timing";
}
