#include "IntersectionController.h"
#include <iostream>
#include <iomanip>
using namespace std;

IntersectionController::IntersectionController(unique_ptr<SignalStrategy> timingStrategy)
    : strategy(std::move(timingStrategy)) {
    emergencyGreenTime = 4;
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
    string currentDirection = light.getCurrentGreenDirection();

    if (emergencyDirection == "NONE") {
        return;
    }

    if (emergencyDirection == currentDirection && light.getCurrentState().find("GREEN") != string::npos) {
        return;
    }

    if (!light.isEmergencyOverrideActive()) {
        light.forceGreen(emergencyDirection, emergencyGreenTime);
        cout << "[Emergency Override] An emergency vehicle was detected in the "
             << emergencyDirection << " direction. The light was temporarily switched to give it priority.\n";
    }
}

void IntersectionController::updateLight() {
    int nsCount = nsSensor.countVehicles(northSouthQueue);
    int ewCount = ewSensor.countVehicles(eastWestQueue);
    auto greenTimes = strategy->getGreenTimes(nsCount, ewCount);

    if (!light.isEmergencyOverrideActive() && light.getCurrentState() == "ALL_RED") {
        string preferredDirection = strategy->choosePriorityDirection(nsCount, ewCount, light.getCurrentGreenDirection());
        light.setNextDirection(preferredDirection);
    }

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
        cout << "Vehicle " << vehicle.getId() << " cleared the intersection going North-South"
             << " | type: " << vehicle.getType()
             << " | waited: " << vehicle.getWaitTime() << " step(s)\n";

        if (vehicle.isEmergencyVehicle() && light.isEmergencyOverrideActive()) {
            if (!nsSensor.detectEmergencyVehicle(northSouthQueue)) {
                light.restoreInterruptedFlow();
                cout << "[Emergency Override Ended] The emergency vehicle has passed. The light returned to the traffic flow that was active before the interruption.\n";
            }
        }
    }
    else if (currentState == "EW_GREEN" && !eastWestQueue.empty()) {
        Vehicle vehicle = eastWestQueue.front();
        eastWestQueue.pop();

        stats.recordProcessedVehicle(vehicle.isEmergencyVehicle(), vehicle.getWaitTime());
        cout << "Vehicle " << vehicle.getId() << " cleared the intersection going East-West"
             << " | type: " << vehicle.getType()
             << " | waited: " << vehicle.getWaitTime() << " step(s)\n";

        if (vehicle.isEmergencyVehicle() && light.isEmergencyOverrideActive()) {
            if (!ewSensor.detectEmergencyVehicle(eastWestQueue)) {
                light.restoreInterruptedFlow();
                cout << "[Emergency Override Ended] The emergency vehicle has passed. The light returned to the traffic flow that was active before the interruption.\n";
            }
        }
    }
}

void IntersectionController::recordQueueSnapshot() {
    stats.recordQueueSnapshot(nsSensor.countVehicles(northSouthQueue),
                              ewSensor.countVehicles(eastWestQueue));
}

void IntersectionController::displayState(int timeStep) const {
    cout << "\n----------------------------------------\n";
    cout << "Step " << timeStep << " snapshot\n";
    cout << "Control mode: " << strategy->getName() << "\n";
    cout << "Current light: " << light.getCurrentState() << "\n";
    cout << "Time remaining in this light: " << light.getTimer() << " step(s)\n";
    cout << "Cars waiting North-South: " << nsSensor.countVehicles(northSouthQueue) << "\n";
    cout << "Cars waiting East-West:   " << ewSensor.countVehicles(eastWestQueue) << "\n";

    if (!northSouthQueue.empty()) {
        cout << "Front North-South vehicle distance from stop line: "
             << fixed << setprecision(1) << northSouthQueue.front().getPosition() << " units\n";
    } else {
        cout << "Front North-South vehicle distance from stop line: none\n";
    }

    if (!eastWestQueue.empty()) {
        cout << "Front East-West vehicle distance from stop line:   "
             << fixed << setprecision(1) << eastWestQueue.front().getPosition() << " units\n";
    } else {
        cout << "Front East-West vehicle distance from stop line:   none\n";
    }

    cout << "----------------------------------------\n";
}

string IntersectionController::getStrategyName() const {
    return strategy->getName();
}

const Statistics& IntersectionController::getStatistics() const {
    return stats;
}
