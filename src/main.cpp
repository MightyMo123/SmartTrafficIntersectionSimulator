#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>
#include "Simulator.h"
#include "SignalStrategy.h"
using namespace std;

static vector<VehicleEvent> buildScenario(int steps) {
    vector<VehicleEvent> events;
    events.reserve(steps);

    for (int i = 0; i < steps; i++) {
        VehicleEvent event;
        event.generated = (rand() % 100 < 70);
        event.direction = (rand() % 2 == 0) ? "NS" : "EW";
        event.emergency = (rand() % 100 < 15);
        events.push_back(event);
    }

    return events;
}

int main() {
    cout << "===== Smart Traffic Intersection Simulator =====\n";

    int steps;
    cout << "Enter number of simulation steps: ";
    cin >> steps;

    if (steps <= 0) {
        cout << "Invalid input. Exiting program.\n";
        return 1;
    }

    srand(static_cast<unsigned int>(time(0)));
    vector<VehicleEvent> scenario = buildScenario(steps);

    cout << "\n===== RUN 1: FIXED-TIMING BASELINE =====\n";
    Simulator fixedSimulator(steps, std::unique_ptr<SignalStrategy>(new FixedTimingStrategy()), scenario);
    fixedSimulator.run();

    cout << "\n===== RUN 2: ADAPTIVE-TIMING OPTIMIZATION =====\n";
    Simulator adaptiveSimulator(steps, std::unique_ptr<SignalStrategy>(new AdaptiveTimingStrategy()), scenario);
    adaptiveSimulator.run();

    cout << "\nSimulation comparison finished successfully.\n";
    return 0;
}
