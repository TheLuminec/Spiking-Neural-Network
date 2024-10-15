#pragma once
/*
#ifndef SPIKERNETWORK_H
#define SPIKERNETWORK_H

#include <vector>
#include <set>
#include <unordered_map>
#include <thread>
#include <chrono>
#include <atomic>
#include <algorithm>

// Custom hash function for using pairs as keys in unordered_map
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        // Combine hash of both pair elements
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

// Represents a connection between two neurons
struct Connection {
    const int from; // ID of the source neuron
    const int to;   // ID of the target neuron
    double weight = 0; // Weight of the connection
    double eligibilityTrace = 0.0; // Eligibility trace for learning

    // Default constructor, initializes to invalid connection
    Connection() : from(-1), to(-1) {}

    // Constructor for random weight generation between min and max
    explicit Connection(int from, int to, double min, double max) : from(from), to(to) {
        weight = (((double)std::rand() / RAND_MAX) * (max - min)) + min;
    }

    // Constructor with a specific weight
    explicit Connection(int from, int to, double weight) : from(from), to(to), weight(weight) {}

    // Comparison operator for using in ordered sets (based on weight)
    bool operator< (const Connection& right) const {
        return weight < right.weight;
    }
};

// Represents a neuron in the network
struct Neuron {
    const int id; // Unique identifier for the neuron
    int threshold = 0; // Threshold for firing (spiking)
    int refractoryPeriod = 0; // Time to wait after a spike before firing again
    int timeSinceLastSpike = 0; // Time since the last spike
    double value = 0.0; // Current potential of the neuron
    bool funcOutput = false; // Flag to indicate if this neuron has a special function
    int funcId = -1; // Function ID if applicable
    void (*func)(SpikerNetwork*, int) = nullptr; // Function pointer for custom behavior

    // Set of outgoing connections from this neuron
    std::set<Connection> paths;

    // Default constructor, initializes to an invalid neuron
    Neuron() : id(-1) {}

    // Constructor with basic neuron properties
    Neuron(int id, int threshold, int refractoryPeriod) :
        id(id), threshold(threshold), refractoryPeriod(refractoryPeriod), funcId(0) {}

    // Constructor with custom function
    Neuron(int id, int threshold, int refractoryPeriod, int funcId, void (*func)(SpikerNetwork*, int)) :
        id(id), threshold(threshold), refractoryPeriod(refractoryPeriod),
        funcId(funcId), func(func), funcOutput(true) {}

    // Check if the neuron is ready to fire (out of refractory period)
    bool ready(int time) const {
        return (time - timeSinceLastSpike) >= refractoryPeriod;
    }

    // Simulates the neuron receiving input and checks if it should spike
    bool spiked(double weight) {
        value += weight; // Add input weight to current potential
        if (value < threshold)
            return false; // Not enough potential to spike
        value = 0.0; // Reset potential after spike
        return true; // Spike!
    }

    // Add a random connection to another neuron
    void addConnection(int to) {
        paths.insert(Connection(id, to, -2.0, 2.0));
    }

    // Add a specific connection to another neuron
    void addConnection(int to, double weight) {
        paths.insert(Connection(id, to, weight));
    }

    // Comparison operator for using in ordered sets (based on ID)
    bool operator< (const Neuron& right) const {
        return id < right.id;
    }
};

// Main class for the spiking neural network
class SpikerNetwork {
private:
    int tickTime = 0; // Current time in ticks
    std::thread loopThread; // Thread for running the network in real-time
    std::atomic<bool> running = false; // Flag to control the loop
    std::vector<int> neuronQueue; // Queue of neurons to process

    // Test function (not used yet)
    virtual void test() {}

    // Main loop for temporal updates, runs at a fixed FPS
    void temporalLoop(const int fps) {
        const std::chrono::milliseconds frameDuration(1000 / fps);

        while (running) {
            auto startTime = std::chrono::high_resolution_clock::now();
            tick(); // Process one time step

            // Calculate the elapsed time and sleep for the remaining frame duration
            auto endTime = std::chrono::high_resolution_clock::now();
            std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

            if (elapsed < frameDuration) {
                std::this_thread::sleep_for(frameDuration - elapsed);
            }
        }
    }

    // Updates the eligibility trace of a connection using the time difference deltaT
    void updateEligibilityTrace(int from, int to, double deltaT) {
        auto& connection = connections[std::make_pair(from, to)];
        const double tau = 20.0; // Time constant for eligibility trace decay
        connection.eligibilityTrace += exp(-abs(deltaT) / tau);
    }

protected:
    // Map of neurons, each with an active flag (true means active)
    std::unordered_map<int, std::pair<Neuron, bool>> neurons;

    // Map of connections between neurons, hashed by (from, to) pair
    std::unordered_map<std::pair<int, int>, Connection, hash_pair> connections;

    // Inserts a fully initialized neuron into the network
    void insertFullNeuron(const Neuron& n) {
        neurons.emplace(n.id, std::make_pair(n, true));
        for (const Connection& c : n.paths) {
            connections.emplace(std::make_pair(c.from, c.to), c);
        }
    }

    // Creates and inserts a basic neuron
    std::pair<Neuron, bool> createNeuronPair(int id, int threshold, int refractoryPeriod) {
        Neuron neuron(id, threshold, refractoryPeriod);
        std::pair<Neuron, bool> newPair(neuron, true);
        neurons.emplace(id, newPair);
        return newPair;
    }

    // Creates and inserts a neuron with a custom function
    std::pair<Neuron, bool> createNeuronPair(int id, int threshold, int refractoryPeriod, int funcId, void (*func)(SpikerNetwork*, int)) {
        Neuron neuron(id, threshold, refractoryPeriod, funcId, func);
        std::pair<Neuron, bool> newPair(neuron, true);
        neurons.emplace(id, newPair);
        return newPair;
    }

    // Fires a neuron, propagating its spike to connected neurons
    void fire(int id) {
        auto& neuron = neurons[id].first;
        if (neuron.funcOutput) { // If neuron has a custom function
            neurons[id].second = false; // Set neuron as inactive
            neuron.timeSinceLastSpike = tickTime;
            (*neuron.func)(this, neuron.funcId); // Call custom function
            return;
        }

        // Process outgoing connections (propagation)
        for (const Connection& c : neuron.paths) {
            auto& targetNeuron = neurons[c.to].first;
            if (!neurons[c.to].second) { // If target neuron is inactive
                if (!targetNeuron.ready(tickTime))
                    continue;
                neurons[c.to].second = true; // Activate the target neuron
            }

            if (targetNeuron.spiked(c.weight)) { // If the target neuron spikes
                neuronQueue.push_back(c.to); // Add target neuron to the queue
                neurons[c.to].second = false; // Set target neuron as inactive
                updateEligibilityTrace(c.from, c.to, tickTime - targetNeuron.timeSinceLastSpike);
                targetNeuron.timeSinceLastSpike = tickTime; // Update spike time
            }
        }
    }

public:
    // Constructor for initializing a random spiking network
    explicit SpikerNetwork() {
        std::srand(time(NULL)); // Seed random number generator
        tickTime = 0;
    }

    // Constructor for initializing the network with a set of neurons
    explicit SpikerNetwork(const std::set<Neuron>& neurons) {
        for (const Neuron& n : neurons) {
            insertFullNeuron(n);
        }
    }

    // Destructor: stops the network loop if running
    ~SpikerNetwork() {
        stopLoop();
    }

    // Sets an output function for all neurons
    void outputSet(void (*func)(SpikerNetwork*, int)) {
        for (auto& n : neurons) {
            n.second.first.func = func;
        }
    }

    // Sets an output function for a specific neuron by ID
    void outputSet(int id, void (*func)(SpikerNetwork*, int)) {
        if (neurons.find(id) == neurons.end())
            throw std::runtime_error("outputSet id does not exist.");
        neurons[id].first.func = func;
    }

    // Queues a neuron for processing in the next tick
    void queueNeuron(int id) {
        neuronQueue.push_back(id);
    }

    // Prepares the neuron queue and clears connection data
    void reserve() {
        neuronQueue.reserve(neurons.size());
        connections.clear();
        for (const auto& n : neurons) {
            for (const Connection& c : n.second.first.paths) {
                connections.emplace(std::pair<int, int>(c.from, c.to), c);
            }
        }
    }

    // Processes one time step
    void tick() {
        tickTime++;
        if (neuronQueue.empty())
            return;

        std::vector<int> tickQueue = neuronQueue;
        neuronQueue.clear();

        // Process each queued neuron
        for (int id : tickQueue) {
            fire(id);
        }
    }

    // Processes one full cycle (same as tick if neurons are queued)
    void fullTick() {
        if (!neuronQueue.empty())
            tick();
    }

    // Advances time by a number of ticks without processing
    void wait(int ticks) {
        tickTime += ticks;
    }

    // Starts the real-time loop at a specific FPS
    void startLoop(const int fps) {
        running = true;
        loopThread = std::thread(&SpikerNetwork::temporalLoop, this, fps);
    }

    // Stops the real-time loop
    void stopLoop() {
        running = false;
        if (loopThread.joinable())
            loopThread.join();
    }

    // Applies a reward signal to adjust connection weights
    void applyReward(double reward) {
        const double A_plus = 0.005; // Learning rate for positive reward
        const double A_minus = -0.005; // Learning rate for negative reward

        for (auto& pair : connections) {
            auto& connection = pair.second;
            double weightChange = (reward > 0 ? A_plus : A_minus) * connection.eligibilityTrace * reward;
            connection.weight += weightChange;
            connection.weight = std::clamp(connection.weight, -2.0, 2.0); // Clamp weight within bounds

            // Decay the eligibility trace
            connection.eligibilityTrace *= 0.9;
        }
    }

    // Returns the current network structure (neurons and connections)
    std::pair<std::vector<Neuron>, std::vector<Connection>> returnNetwork() const {
        std::vector<Neuron> neuronList;
        std::vector<Connection> connectionList;

        // Collect all neurons
        for (const auto& pair : neurons) {
            neuronList.push_back(pair.second.first);
        }

        // Collect all connections
        for (const auto& pair : connections) {
            connectionList.push_back(pair.second);
        }

        return std::make_pair(neuronList, connectionList);
    }
};

#endif // !SPIKERNETWORK_H
*/