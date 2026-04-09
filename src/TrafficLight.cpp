#include "TrafficLight.h"

TrafficLight::TrafficLight() {
    currentState = "NS_GREEN";
    timer = 5;
    yellowDuration = 2;
    allRedDuration = 1;
    nextDirection = "EW";
}

string TrafficLight::getCurrentState() const {
    return currentState;
}

int TrafficLight::getTimer() const {
    return timer;
}

void TrafficLight::forceGreen(string priorityDirection, int greenTime) {
    currentState = priorityDirection + "_GREEN";
    timer = greenTime;
    nextDirection = (priorityDirection == "NS") ? "EW" : "NS";
}

void TrafficLight::update(int nsGreenTime, int ewGreenTime) {
    timer--;

    if (timer > 0) {
        return;
    }

    if (currentState == "NS_GREEN") {
        currentState = "NS_YELLOW";
        timer = yellowDuration;
    }
    else if (currentState == "NS_YELLOW") {
        currentState = "ALL_RED";
        timer = allRedDuration;
        nextDirection = "EW";
    }
    else if (currentState == "EW_GREEN") {
        currentState = "EW_YELLOW";
        timer = yellowDuration;
    }
    else if (currentState == "EW_YELLOW") {
        currentState = "ALL_RED";
        timer = allRedDuration;
        nextDirection = "NS";
    }
    else if (currentState == "ALL_RED") {
        if (nextDirection == "NS") {
            currentState = "NS_GREEN";
            timer = nsGreenTime;
        }
        else {
            currentState = "EW_GREEN";
            timer = ewGreenTime;
        }
    }
}