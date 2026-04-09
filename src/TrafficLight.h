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

public:
    TrafficLight();

    string getCurrentState() const;
    int getTimer() const;

    void forceGreen(string priorityDirection, int greenTime);
    void update(int nsGreenTime, int ewGreenTime);
};

#endif