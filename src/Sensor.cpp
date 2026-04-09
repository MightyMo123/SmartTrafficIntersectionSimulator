#include "Sensor.h"

bool Sensor::detectVehicle(queue<Vehicle> q) const {
    return !q.empty();
}

bool Sensor::detectEmergencyVehicle(queue<Vehicle> q) const {
    while (!q.empty()) {
        if (q.front().isEmergencyVehicle()) {
            return true;
        }
        q.pop();
    }
    return false;
}

int Sensor::countVehicles(queue<Vehicle> q) const {
    return static_cast<int>(q.size());
}