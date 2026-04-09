#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
using namespace std;

class Vehicle {
private:
    int id;
    string direction;     // "NS" or "EW"
    bool emergency;
    double position;      // simulated distance from stop line
    double speed;         // movement per step
    int waitTime;

public:
    Vehicle(int vehicleId, string dir, bool isEmergency);

    int getId() const;
    string getDirection() const;
    bool isEmergencyVehicle() const;
    string getType() const;

    double getPosition() const;
    double getSpeed() const;
    int getWaitTime() const;

    void move();
    void stopAtLine();
    void incrementWaitTime();
};

#endif
