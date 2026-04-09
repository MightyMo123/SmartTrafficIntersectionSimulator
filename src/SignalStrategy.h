#ifndef SIGNALSTRATEGY_H
#define SIGNALSTRATEGY_H

#include <string>
#include <utility>

class SignalStrategy {
public:
    virtual ~SignalStrategy() = default;
    virtual std::pair<int, int> getGreenTimes(int nsCount, int ewCount) const = 0;
    virtual std::string getName() const = 0;
};

class FixedTimingStrategy : public SignalStrategy {
private:
    int greenTime;

public:
    explicit FixedTimingStrategy(int fixedGreenTime = 5);
    std::pair<int, int> getGreenTimes(int nsCount, int ewCount) const override;
    std::string getName() const override;
};

class AdaptiveTimingStrategy : public SignalStrategy {
private:
    int baseGreenTime;
    int extraGreenTime;

public:
    AdaptiveTimingStrategy(int base = 5, int extra = 3);
    std::pair<int, int> getGreenTimes(int nsCount, int ewCount) const override;
    std::string getName() const override;
};

#endif
