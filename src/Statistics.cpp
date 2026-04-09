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
    out << "\n========== PERFORMANCE SUMMARY: " << strategyName << " ==========\n";
    out << "Total vehicles generated: " << totalGenerated << "\n";
    out << "Total vehicles processed: " << totalProcessed << "\n";
    out << "Emergency vehicles processed: " << emergencyProcessed << "\n";
    out << std::fixed << std::setprecision(2);
    out << "Average wait time: " << getAverageWaitTime() << " steps\n";
    out << "Average NS queue length: " << getAverageNSQueueLength() << "\n";
    out << "Average EW queue length: " << getAverageEWQueueLength() << "\n";
    out << "Max NS queue length: " << maxNSQueueLength << "\n";
    out << "Max EW queue length: " << maxEWQueueLength << "\n";
    out << "Throughput: " << getThroughputPercent() << "%\n";
    out << "========================================================\n";
    return out.str();
}
