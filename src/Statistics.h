#ifndef STATISTICS_H
#define STATISTICS_H

#include <string>

class Statistics {
private:
    int totalGenerated;
    int totalProcessed;
    int emergencyProcessed;
    long long totalWaitTime;
    long long cumulativeNSQueueLength;
    long long cumulativeEWQueueLength;
    int maxNSQueueLength;
    int maxEWQueueLength;
    int samples;

public:
    Statistics();

    void recordGeneratedVehicle();
    void recordProcessedVehicle(bool emergency, int waitTime);
    void recordQueueSnapshot(int nsCount, int ewCount);

    int getTotalGenerated() const;
    int getTotalProcessed() const;
    int getEmergencyProcessed() const;
    double getAverageWaitTime() const;
    double getAverageNSQueueLength() const;
    double getAverageEWQueueLength() const;
    int getMaxNSQueueLength() const;
    int getMaxEWQueueLength() const;
    double getThroughputPercent() const;

    std::string summary(const std::string& strategyName) const;
};

#endif
