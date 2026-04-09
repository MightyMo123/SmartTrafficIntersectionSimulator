#include "Simulator.h"
#include <iostream>
using namespace std;

Simulator::Simulator(int steps, unique_ptr<SignalStrategy> strategy, const vector<VehicleEvent>& events)
    : controller(std::move(strategy)), scheduledEvents(events) {
    currentTimeStep = 1;
    maxSteps = steps;
    nextVehicleId = 1;
}

void Simulator::generateVehicle() {
    const VehicleEvent& event = scheduledEvents[currentTimeStep - 1];

    if (!event.generated) {
        cout << "No vehicle generated this step.\n";
        return;
    }

    Vehicle newVehicle(nextVehicleId, event.direction, event.emergency);
    controller.addVehicle(newVehicle);

    cout << "Generated Vehicle ID " << nextVehicleId
         << " | Direction: " << newVehicle.getDirection()
         << " | Type: " << newVehicle.getType()
         << " | Start Position: " << newVehicle.getPosition() << endl;

    nextVehicleId++;
}

void Simulator::run() {
    while (currentTimeStep <= maxSteps) {
        cout << "\n----- SIMULATION STEP " << currentTimeStep << " -----\n";

        generateVehicle();
        controller.handleEmergencyOverride();
        controller.updateLight();
        controller.updateVehiclePositions();
        controller.processTraffic();
        controller.recordQueueSnapshot();
        controller.displayState(currentTimeStep);

        currentTimeStep++;
    }

    cout << controller.getStatistics().summary(controller.getStrategyName());
}
