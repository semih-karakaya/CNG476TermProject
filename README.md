# CNG476TermProject

# Smart Bus Terminal LoRaWAN Simulation with OMNeT++ and FLoRa

This project simulates a smart bus and bus terminal network using the LoRa wireless communication protocol. The simulation is implemented with [OMNeT++](https://omnetpp.org/) and the [FLoRa](https://flora.aalto.fi/) simulation framework, which lets us model long-range, low-power LoRa communications realistically.

## Project Overview

In this simulation, infrared sensors are used at bus terminals to count the number of passengers waiting, and on buses to detect passenger entry and exit. These sensors communicate data over LoRaWAN to a central monitoring hub. The central hub receives and processes:
- The number of waiting passengers at stops
- Bus occupancy and availability

The system aims to optimize public transportation by providing real-time updates and improving passenger flow management.

## Objectives

- Analyze network latency and energy efficiency as sensors send data about passengers and buses over LoRaWAN.
- Measure the reliability and accuracy of LoRa-based communication for critical transit information.
- Study the effects of different passenger arrival rates and bus dispatching strategies.
- Evaluate LoRa's performance for smart transport compared to traditional Wi-Fi-based IoT systems.

## Simulation Approach

- **Random (stochastic) models** are used: Passenger arrivals are modeled as random events using probability distributions (like Poisson and exponential distributions), reflecting real-world situations.
- The simulation uses **stochastic processes** to track how the network state (e.g. queue size, bus occupancy) changes over time.
- **Monte Carlo simulation**: The simulation is repeated many times with different random inputs, allowing us to obtain reliable averages for network latency, energy usage, and communication reliability.

## Requirements

- [OMNeT++ 6.0](https://omnetpp.org/download/)
- [FLoRa framework](https://flora.aalto.fi/) (Add-on for OMNeT++)
- C++ compiler compatible with OMNeT++
  

