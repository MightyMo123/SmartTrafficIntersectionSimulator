# 🚦 Smart Traffic Intersection Simulator

## 👥 Team Members
- Mohamed Hagyousif
- Daniel Jean-Louis
- Zhaojun Jiang

---

## 📌 Project Overview
The Smart Traffic Intersection Simulator is a C++ application that models a two-direction intersection (North-South and East-West) using object-oriented design, a finite state traffic light controller, queue-based vehicle flow, emergency vehicle priority, and performance analysis.

The updated version now supports both a **fixed-timing baseline** and an **adaptive-timing strategy**, allowing the team to directly compare signal control methods using the same traffic scenario.

---

## ⚙️ Main Functionalities

### 🚗 Vehicle Generation
Vehicles are generated from a prebuilt scenario so that multiple control strategies can be tested fairly on identical traffic conditions.

Each generated vehicle has:
- Direction (NS or EW)
- Type (Regular or Emergency)
- Position and speed
- Accumulated wait time

### 🚦 Queue Management & Movement
- Vehicles are stored in direction-specific queues
- Front vehicles move when their direction is green
- Vehicles accumulate wait time while stopped
- Vehicles stop at the stop line during non-green phases

### 📡 Sensor Detection
Sensors monitor each direction and report:
- Whether vehicles are present
- Whether an emergency vehicle is present
- Total queue length

### 🔁 Traffic Light Control (FSM)
The signal follows a finite state machine:

NS_GREEN → NS_YELLOW → ALL_RED → EW_GREEN → EW_YELLOW → ALL_RED → repeat

This keeps the simulation safe and realistic during phase transitions.

### ⏱️ Signal Optimization Comparison
The simulator supports two timing strategies:
- **FixedTimingStrategy**: both directions receive the same green duration
- **AdaptiveTimingStrategy**: the direction with the larger queue receives extra green time

This satisfies the optimization requirement by enabling direct comparison between baseline and improved control logic.

### 🚨 Emergency Vehicle Override
If a sensor detects an emergency vehicle in a blocked direction, the controller forces that direction to green for a priority window.

### 📊 Performance Statistics
The simulator now tracks:
- Total vehicles generated
- Total vehicles processed
- Emergency vehicles processed
- Average wait time
- Average queue length for NS and EW
- Maximum queue length for NS and EW
- Throughput percentage

At the end of each run, a summary report is printed for that strategy.

---

## 🧠 Object-Oriented Design

The system is built around multiple interacting classes:

- `Vehicle` → models each vehicle and its wait time / motion
- `TrafficLight` → manages FSM states and timing transitions
- `Sensor` → detects queue status and emergency conditions
- `SignalStrategy` → abstract base class for traffic timing logic
- `FixedTimingStrategy` → derived strategy for constant green timing
- `AdaptiveTimingStrategy` → derived strategy for queue-aware timing
- `Statistics` → records performance metrics and generates summaries
- `IntersectionController` → coordinates vehicles, signals, sensors, and statistics
- `Simulator` → runs a complete simulation using a selected strategy

### OOP Principles Demonstrated
- **Encapsulation** through class-based state and methods
- **Abstraction** through clear class responsibilities
- **Inheritance** via timing strategies derived from `SignalStrategy`
- **Polymorphism** by swapping timing strategies through a base-class pointer

---

## 🛠️ Technologies Used
- C++
- Standard Template Library (STL)
  - `queue`
  - `vector`
  - `memory`
  - `utility`
- Command-line interface
- Git & GitHub

---

## 📁 Project Structure

SmartTrafficIntersectionSimulator/
│
├── README.md
├── docs/
│   └── System_Design_Overview.pdf
├── pseudocode/
│   └── project_pseudocode.txt
├── src/
│   ├── main.cpp
│   ├── Vehicle.h / Vehicle.cpp
│   ├── TrafficLight.h / TrafficLight.cpp
│   ├── Sensor.h / Sensor.cpp
│   ├── SignalStrategy.h / SignalStrategy.cpp
│   ├── Statistics.h / Statistics.cpp
│   ├── IntersectionController.h / IntersectionController.cpp
│   └── Simulator.h / Simulator.cpp
└── images/
    └── system_diagram.png

---

## ▶️ How to Run the Program

### Compile
```bash
g++ -std=c++14 main.cpp Vehicle.cpp TrafficLight.cpp Sensor.cpp SignalStrategy.cpp Statistics.cpp IntersectionController.cpp Simulator.cpp -o traffic_sim
```

### Run
```bash
./traffic_sim
```

The program will:
1. Generate a shared traffic scenario
2. Run the fixed-timing baseline
3. Run the adaptive-timing strategy
4. Print a performance summary for each

---

## 🎯 Project Goals Addressed
- Build a working multi-class C++ traffic simulator
- Apply OOP principles including inheritance and polymorphism
- Generate meaningful simulation statistics
- Demonstrate signal optimization by comparing fixed and adaptive timing
- Collaborate through a structured GitHub repository
- Document design decisions and engineering tradeoffs

---

## 🧪 Engineering Decisions and Tradeoffs
- A **shared scenario generator** was added so both strategies could be compared fairly under identical traffic input.
- A **strategy-pattern design** was chosen because it cleanly separates timing logic from intersection control.
- Statistics were tracked inside a dedicated class to keep the controller focused on behavior rather than reporting.
- The simulator remains text-based to prioritize control logic, correctness, and modularity over graphics.

---

## 🚀 Future Improvements
- Simulate movement for all vehicles in the queue, not only the front vehicle
- Add left-turn lanes or multiple lanes
- Add GUI-based visualization
- Export simulation statistics to CSV for graphing
- Add smarter emergency recovery after override mode

---

## 📎 Notes
This project models real-world traffic engineering concepts in software form and is intended to demonstrate simulation design, signal logic, optimization, and OOP-based implementation.
