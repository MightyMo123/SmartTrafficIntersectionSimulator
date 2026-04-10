#include "TrafficLight.h"

TrafficLight::TrafficLight() {
    currentState = "NS_GREEN";
    timer = 5;
    yellowDuration = 2;
    allRedDuration = 1;
    nextDirection = "EW";
    emergencyOverrideActive = false;
    emergencyDirection = "NONE";
    savedDirection = "NS";
    savedGreenRemaining = 3;
}

string TrafficLight::getCurrentState() const {
    return currentState;
}

int TrafficLight::getTimer() const {
    return timer;
}

string TrafficLight::getCurrentGreenDirection() const {
    if (currentState.rfind("NS", 0) == 0) {
        return "NS";
    }
    if (currentState.rfind("EW", 0) == 0) {
        return "EW";
    }
    return nextDirection;
}

bool TrafficLight::isEmergencyOverrideActive() const {
    return emergencyOverrideActive;
}

void TrafficLight::setNextDirection(const string& preferredDirection) {
    if (preferredDirection == "NS" || preferredDirection == "EW") {
        nextDirection = preferredDirection;
    }
}

void TrafficLight::forceGreen(const string& priorityDirection, int greenTime) {
    if (!emergencyOverrideActive) {
        savedDirection = getCurrentGreenDirection();
        savedGreenRemaining = (currentState.find("GREEN") != string::npos) ? timer : 3;
        if (savedGreenRemaining < 2) {
            savedGreenRemaining = 2;
        }
    }

    emergencyOverrideActive = true;
    emergencyDirection = priorityDirection;
    currentState = priorityDirection + "_GREEN";
    timer = greenTime;
}

void TrafficLight::restoreInterruptedFlow() {
    if (!emergencyOverrideActive) {
        return;
    }

    currentState = savedDirection + "_GREEN";
    timer = savedGreenRemaining;
    nextDirection = (savedDirection == "NS") ? "EW" : "NS";
    emergencyOverrideActive = false;
    emergencyDirection = "NONE";
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
    }
    else if (currentState == "EW_GREEN") {
        currentState = "EW_YELLOW";
        timer = yellowDuration;
    }
    else if (currentState == "EW_YELLOW") {
        currentState = "ALL_RED";
        timer = allRedDuration;
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
