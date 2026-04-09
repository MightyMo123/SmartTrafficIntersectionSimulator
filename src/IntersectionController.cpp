#include "IntersectionController.h"
#include <iostream>
using namespace std;

IntersectionController::IntersectionController(unique_ptr<SignalStrategy> timingStrategy)
    : strategy(std::move(timingStrategy)) {
    emergencyGreenTime = 6;
}

void IntersectionController::addVehicle(const Vehicle& vehicle) {
    if (vehicle.getDirection() == "NS") {
        northSouthQueue.push(vehicle);
    }
    else if (vehicle.getDirection() == "EW") {
        eastWestQueue.push(vehicle);
    }

    stats.recordGeneratedVehicle();
}

string IntersectionController::detectEmergencyDirection() const {
    if (nsSensor.detectEmergencyVehicle(northSouthQueue)) {
        return "NS";
    }
    if (ewSensor.detectEmergencyVehicle(eastWestQueue)) {
        return "EW";
    }
    return "NONE";
}

void IntersectionController::handleEmergencyOverride() {
    string emergencyDirection = detectEmergencyDirection();
    string currentState = light.getCurrentState();

    if (emergencyDirection == "NONE") {
        return;
    }

    if ((emergencyDirection == "NS" && currentState == "NS_GREEN") ||
        (emergencyDirection == "EW" && currentState == "EW_GREEN")) {
        return;
    }

    light.forceGreen(emergencyDirection, emergencyGreenTime);
    cout << ">>> EMERGENCY OVERRIDE ACTIVATED for " << emergencyDirection << " direction!\n";
}

void IntersectionController::updateLight() {
    int nsCount = nsSensor.countVehicles(northSouthQueue);
    int ewCount = ewSensor.countVehicles(eastWestQueue);
    auto greenTimes = strategy->getGreenTimes(nsCount, ewCount);
    light.update(greenTimes.first, greenTimes.second);
}

void IntersectionController::incrementWaitingVehicles(queue<Vehicle>& q) {
    queue<Vehicle> updated;

    while (!q.empty()) {
        Vehicle current = q.front();
        q.pop();
        current.incrementWaitTime();
        updated.push(current);
    }

    q = updated;
}

void IntersectionController::updateVehiclePositions() {
    bool nsGreen = (light.getCurrentState() == "NS_GREEN");
    bool ewGreen = (light.getCurrentState() == "EW_GREEN");

    if (!nsGreen) {
        incrementWaitingVehicles(northSouthQueue);
    }
    if (!ewGreen) {
        incrementWaitingVehicles(eastWestQueue);
    }

    if (!northSouthQueue.empty()) {
        Vehicle front = northSouthQueue.front();
        northSouthQueue.pop();

        if (nsGreen) {
            front.move();
        } else {
            front.stopAtLine();
        }

        northSouthQueue.push(front);
    }

    if (!eastWestQueue.empty()) {
        Vehicle front = eastWestQueue.front();
        eastWestQueue.pop();

        if (ewGreen) {
            front.move();
        } else {
            front.stopAtLine();
        }

        eastWestQueue.push(front);
    }
}

void IntersectionController::processTraffic() {
    string currentState = light.getCurrentState();

    if (currentState == "NS_GREEN" && !northSouthQueue.empty()) {
        Vehicle vehicle = northSouthQueue.front();
        northSouthQueue.pop();

        stats.recordProcessedVehicle(vehicle.isEmergencyVehicle(), vehicle.getWaitTime());
        cout << "Vehicle ID " << vehicle.getId()
             << " (" << vehicle.getType() << ") passed in NS direction after waiting "
             << vehicle.getWaitTime() << " step(s).\n";
    }
    else if (currentState == "EW_GREEN" && !eastWestQueue.empty()) {
        Vehicle vehicle = eastWestQueue.front();
        eastWestQueue.pop();

        stats.recordProcessedVehicle(vehicle.isEmergencyVehicle(), vehicle.getWaitTime());
        cout << "Vehicle ID " << vehicle.getId()
             << " (" << vehicle.getType() << ") passed in EW direction after waiting "
             << vehicle.getWaitTime() << " step(s).\n";
    }
}

void IntersectionController::recordQueueSnapshot() {
    stats.recordQueueSnapshot(nsSensor.countVehicles(northSouthQueue),
                              ewSensor.countVehicles(eastWestQueue));
}

void IntersectionController::displayState(int timeStep) const {
    cout << "\n========================================\n";
    cout << "Time Step: " << timeStep << endl;
    cout << "Control Strategy: " << strategy->getName() << endl;
    cout << "Traffic Light State: " << light.getCurrentState() << endl;
    cout << "Light Timer: " << light.getTimer() << endl;
    cout << "NS Sensor Count: " << nsSensor.countVehicles(northSouthQueue) << endl;
    cout << "EW Sensor Count: " << ewSensor.countVehicles(eastWestQueue) << endl;

    if (!northSouthQueue.empty()) {
        cout << "Front NS Vehicle Position: " << northSouthQueue.front().getPosition() << endl;
    } else {
        cout << "Front NS Vehicle Position: none" << endl;
    }

    if (!eastWestQueue.empty()) {
        cout << "Front EW Vehicle Position: " << eastWestQueue.front().getPosition() << endl;
    } else {
        cout << "Front EW Vehicle Position: none" << endl;
    }

    cout << "========================================\n";
}

string IntersectionController::getStrategyName() const {
    return strategy->getName();
}

const Statistics& IntersectionController::getStatistics() const {
    return stats;
}
