#ifndef SIGNALSTRATEGY_H
#define SIGNALSTRATEGY_H

#include <string>
#include <utility>

class SignalStrategy {
public:
    virtual ~SignalStrategy() = default;
    virtual std::pair<int, int> getGreenTimes(int nsCount, int ewCount) const = 0;
    virtual std::string choosePriorityDirection(int nsCount, int ewCount, const std::string& currentGreenDirection) const = 0;
    virtual std::string getName() const = 0;
};

class FixedTimingStrategy : public SignalStrategy {
private:
    int greenTime;

public:
    explicit FixedTimingStrategy(int fixedGreenTime = 5);
    std::pair<int, int> getGreenTimes(int nsCount, int ewCount) const override;
    std::string choosePriorityDirection(int nsCount, int ewCount, const std::string& currentGreenDirection) const override;
    std::string getName() const override;
};

class AdaptiveTimingStrategy : public SignalStrategy {
private:
    int minGreenTime;
    int baseGreenTime;
    int maxGreenTime;
    int threshold;

public:
    AdaptiveTimingStrategy(int minGreen = 3, int base = 5, int maxGreen = 8, int switchThreshold = 2);
    std::pair<int, int> getGreenTimes(int nsCount, int ewCount) const override;
    std::string choosePriorityDirection(int nsCount, int ewCount, const std::string& currentGreenDirection) const override;
    std::string getName() const override;
};

#endif
