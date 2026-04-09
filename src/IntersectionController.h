#ifndef INTERSECTIONCONTROLLER_H
#define INTERSECTIONCONTROLLER_H

#include <memory>
#include <queue>
#include <string>
#include "Vehicle.h"
#include "TrafficLight.h"
#include "Sensor.h"
#include "SignalStrategy.h"
#include "Statistics.h"
using namespace std;

class IntersectionController {
private:
    queue<Vehicle> northSouthQueue;
    queue<Vehicle> eastWestQueue;

    TrafficLight light;
    Sensor nsSensor;
    Sensor ewSensor;

    unique_ptr<SignalStrategy> strategy;
    Statistics stats;
    int emergencyGreenTime;

public:
    explicit IntersectionController(unique_ptr<SignalStrategy> timingStrategy);

    void addVehicle(const Vehicle& vehicle);
    void handleEmergencyOverride();
    void updateLight();
    void processTraffic();
    void updateVehiclePositions();
    void displayState(int timeStep) const;
    void recordQueueSnapshot();

    string getStrategyName() const;
    const Statistics& getStatistics() const;

private:
    string detectEmergencyDirection() const;
    void incrementWaitingVehicles(queue<Vehicle>& q);
};

#endif
