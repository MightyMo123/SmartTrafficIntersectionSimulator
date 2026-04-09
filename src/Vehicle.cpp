#include "Vehicle.h"

Vehicle::Vehicle(int vehicleId, string dir, bool isEmergency) {
    id = vehicleId;
    direction = dir;
    emergency = isEmergency;
    position = 10.0; // starts 10 units away from the stop line
    speed = isEmergency ? 2.5 : 1.5;
    waitTime = 0;
}

int Vehicle::getId() const {
    return id;
}

string Vehicle::getDirection() const {
    return direction;
}

bool Vehicle::isEmergencyVehicle() const {
    return emergency;
}

string Vehicle::getType() const {
    return emergency ? "Emergency" : "Regular";
}

double Vehicle::getPosition() const {
    return position;
}

double Vehicle::getSpeed() const {
    return speed;
}

int Vehicle::getWaitTime() const {
    return waitTime;
}

void Vehicle::move() {
    position -= speed;
    if (position < 0.0) {
        position = 0.0;
    }
}

void Vehicle::stopAtLine() {
    if (position < 1.0) {
        position = 1.0;
    }
}

void Vehicle::incrementWaitTime() {
    waitTime++;
}
