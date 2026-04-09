#ifndef SENSOR_H
#define SENSOR_H

#include <queue>
#include "Vehicle.h"
using namespace std;

class Sensor {
public:
    bool detectVehicle(queue<Vehicle> q) const;
    bool detectEmergencyVehicle(queue<Vehicle> q) const;
    int countVehicles(queue<Vehicle> q) const;
};

#endif