#include "Statistics.h"
#include <sstream>
#include <iomanip>

Statistics::Statistics() {
    totalGenerated = 0;
    totalProcessed = 0;
    emergencyProcessed = 0;
    totalWaitTime = 0;
    cumulativeNSQueueLength = 0;
    cumulativeEWQueueLength = 0;
    maxNSQueueLength = 0;
    maxEWQueueLength = 0;
    samples = 0;
}

void Statistics::recordGeneratedVehicle() {
    totalGenerated++;
}

void Statistics::recordProcessedVehicle(bool emergency, int waitTime) {
    totalProcessed++;
    totalWaitTime += waitTime;
    if (emergency) {
        emergencyProcessed++;
    }
}

void Statistics::recordQueueSnapshot(int nsCount, int ewCount) {
    cumulativeNSQueueLength += nsCount;
    cumulativeEWQueueLength += ewCount;
    if (nsCount > maxNSQueueLength) maxNSQueueLength = nsCount;
    if (ewCount > maxEWQueueLength) maxEWQueueLength = ewCount;
    samples++;
}

int Statistics::getTotalGenerated() const { return totalGenerated; }
int Statistics::getTotalProcessed() const { return totalProcessed; }
int Statistics::getEmergencyProcessed() const { return emergencyProcessed; }

double Statistics::getAverageWaitTime() const {
    return totalProcessed == 0 ? 0.0 : static_cast<double>(totalWaitTime) / totalProcessed;
}

double Statistics::getAverageNSQueueLength() const {
    return samples == 0 ? 0.0 : static_cast<double>(cumulativeNSQueueLength) / samples;
}

double Statistics::getAverageEWQueueLength() const {
    return samples == 0 ? 0.0 : static_cast<double>(cumulativeEWQueueLength) / samples;
}

int Statistics::getMaxNSQueueLength() const { return maxNSQueueLength; }
int Statistics::getMaxEWQueueLength() const { return maxEWQueueLength; }

double Statistics::getThroughputPercent() const {
    return totalGenerated == 0 ? 0.0 : (100.0 * totalProcessed) / totalGenerated;
}

std::string Statistics::summary(const std::string& strategyName) const {
    std::ostringstream out;
    out << "\n==================== Final Results: " << strategyName << " ====================\n";
    out << "Total vehicles created:           " << totalGenerated << "\n";
    out << "Total vehicles that got through:  " << totalProcessed << "\n";
    out << "Emergency vehicles that got through: " << emergencyProcessed << "\n";
    out << std::fixed << std::setprecision(2);
    out << "Average wait time:                " << getAverageWaitTime() << " step(s)\n";
    out << "Average North-South queue:        " << getAverageNSQueueLength() << "\n";
    out << "Average East-West queue:          " << getAverageEWQueueLength() << "\n";
    out << "Longest North-South queue:        " << maxNSQueueLength << "\n";
    out << "Longest East-West queue:          " << maxEWQueueLength << "\n";
    out << "Throughput:                       " << getThroughputPercent() << "%\n";
    out << "================================================================\n";
    return out.str();
}
