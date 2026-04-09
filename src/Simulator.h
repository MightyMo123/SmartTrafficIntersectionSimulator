#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <memory>
#include <string>
#include <vector>
#include "IntersectionController.h"
#include "SignalStrategy.h"

struct VehicleEvent {
    bool generated;
    std::string direction;
    bool emergency;
};

class Simulator {
private:
    IntersectionController controller;
    int currentTimeStep;
    int maxSteps;
    int nextVehicleId;
    std::vector<VehicleEvent> scheduledEvents;

public:
    Simulator(int steps, std::unique_ptr<SignalStrategy> strategy, const std::vector<VehicleEvent>& events);
    void run();

private:
    void generateVehicle();
};

#endif
