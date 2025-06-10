# CNG476TermProject

## Smart Bus Terminal Network Simulation with OMNeT++

This project simulates a smart bus and bus terminal network using the [OMNeT++](https://omnetpp.org/) network simulation framework. The system models communication between bus stops and a central logic to support smart transportation planning.

### Project Overview

- **Bus stops** periodically generate passengers using a Poisson distribution to reflect real-world arrival behavior.
- Each stop uses **self-messages** to schedule future passenger arrivals.
- A **central controller (or bus logic)** monitors the stops and dispatches a bus to the one with the most passengers.
- When a bus arrives at a stop, it **clears (serves) all waiting passengers** at once.
- No actual tracking of boarding events was implemented due to complexity and time constraints.

This setup simulates real-time optimization where bus resources are dynamically assigned based on demand.

### Objectives

- Analyze communication delay and system responsiveness to passenger accumulation.
- Model realistic passenger arrival behavior using probabilistic methods.
- Test dynamic bus dispatching logic under varying traffic conditions.

### Simulation Approach

- **Stochastic modeling**: Passenger arrivals at each stop follow a Poisson distribution.
- **Discrete-event simulation**: OMNeT++ handles scheduled self-messages and interactions between buses and stops.
- **Centralized dispatch**: The simulation identifies the stop with the most passengers and routes the next bus there.

### Requirements

- [OMNeT++ 6.0](https://omnetpp.org/download/)
- C++ compiler compatible with OMNeT++

### Running the Simulation

To run the simulation:

1. Open the project in OMNeT++.
2. Run the simulation using the `omnetpp.ini` file, which contains all scenario configurations.
