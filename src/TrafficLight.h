#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>
using namespace std;

class TrafficLight {
private:
    string currentState;
    int timer;
    int yellowDuration;
    int allRedDuration;
    string nextDirection;

    bool emergencyOverrideActive;
    string emergencyDirection;
    string savedDirection;
    int savedGreenRemaining;

public:
    TrafficLight();

    string getCurrentState() const;
    int getTimer() const;
    string getCurrentGreenDirection() const;
    bool isEmergencyOverrideActive() const;

    void setNextDirection(const string& preferredDirection);
    void forceGreen(const string& priorityDirection, int greenTime);
    void restoreInterruptedFlow();
    void update(int nsGreenTime, int ewGreenTime);
};

#endif
