#include "SignalStrategy.h"
#include <algorithm>

FixedTimingStrategy::FixedTimingStrategy(int fixedGreenTime) {
    greenTime = fixedGreenTime;
}

std::pair<int, int> FixedTimingStrategy::getGreenTimes(int, int) const {
    return {greenTime, greenTime};
}

std::string FixedTimingStrategy::choosePriorityDirection(int, int, const std::string& currentGreenDirection) const {
    return (currentGreenDirection == "NS") ? "EW" : "NS";
}

std::string FixedTimingStrategy::getName() const {
    return "Fixed Timing";
}

AdaptiveTimingStrategy::AdaptiveTimingStrategy(int minGreen, int base, int maxGreen, int switchThreshold) {
    minGreenTime = minGreen;
    baseGreenTime = base;
    maxGreenTime = maxGreen;
    threshold = switchThreshold;
}

std::pair<int, int> AdaptiveTimingStrategy::getGreenTimes(int nsCount, int ewCount) const {
    int diff = nsCount - ewCount;

    int nsGreen = baseGreenTime + std::max(0, diff);
    int ewGreen = baseGreenTime + std::max(0, -diff);

    nsGreen = std::max(minGreenTime, std::min(maxGreenTime, nsGreen));
    ewGreen = std::max(minGreenTime, std::min(maxGreenTime, ewGreen));

    return {nsGreen, ewGreen};
}

std::string AdaptiveTimingStrategy::choosePriorityDirection(int nsCount, int ewCount, const std::string& currentGreenDirection) const {
    if (nsCount >= ewCount + threshold) {
        return "NS";
    }
    if (ewCount >= nsCount + threshold) {
        return "EW";
    }

    if (nsCount == 0 && ewCount > 0) {
        return "EW";
    }
    if (ewCount == 0 && nsCount > 0) {
        return "NS";
    }

    return (currentGreenDirection == "NS") ? "EW" : "NS";
}

std::string AdaptiveTimingStrategy::getName() const {
    return "Adaptive Timing";
}
