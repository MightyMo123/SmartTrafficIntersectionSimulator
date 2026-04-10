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
        cout << "No new vehicle arrived during this step.\n";
        return;
    }

    Vehicle newVehicle(nextVehicleId, event.direction, event.emergency);
    controller.addVehicle(newVehicle);

    cout << "New vehicle added"
         << " | ID: " << nextVehicleId
         << " | direction: " << ((newVehicle.getDirection() == "NS") ? "North-South" : "East-West")
         << " | type: " << newVehicle.getType()
         << " | starting distance: " << newVehicle.getPosition() << " units\n";

    nextVehicleId++;
}

void Simulator::run() {
    while (currentTimeStep <= maxSteps) {
        cout << "\n========== Simulation Step " << currentTimeStep << " ==========" << "\n";

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
