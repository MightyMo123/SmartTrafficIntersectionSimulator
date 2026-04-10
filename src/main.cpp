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
    cout << "This program runs the same traffic scenario twice:\n";
    cout << "1. Fixed timing\n";
    cout << "2. Adaptive timing\n\n";

    int steps;
    cout << "Enter how many simulation steps you want to run: ";
    cin >> steps;

    if (steps <= 0) {
        cout << "That input is not valid. Program will now stop.\n";
        return 1;
    }

    srand(static_cast<unsigned int>(time(0)));
    vector<VehicleEvent> scenario = buildScenario(steps);

    cout << "\n===== Run 1: Fixed Timing Baseline =====\n";
    Simulator fixedSimulator(steps, std::unique_ptr<SignalStrategy>(new FixedTimingStrategy()), scenario);
    fixedSimulator.run();

    cout << "\n===== Run 2: Adaptive Timing =====\n";
    Simulator adaptiveSimulator(steps, std::unique_ptr<SignalStrategy>(new AdaptiveTimingStrategy()), scenario);
    adaptiveSimulator.run();

    cout << "\nBoth runs are complete. Compare the final results above.\n";
    return 0;
}
