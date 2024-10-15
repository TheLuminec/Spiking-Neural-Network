#ifndef NEURON_H
#define NEURON_H

#include <unordered_set>

class Synapse;

class Neuron {
public:
    Neuron(double threshold, double resetPotential, double restingPotential, double membraneResistance, double membraneTimeConstant);

    void receiveCurrent(double current);
    void update(double timeStep);
    bool hasSpiked() const;
    double getPotential() const;

    void addOutgoingSynapse(Synapse* synapse);

private:
    double membranePotential;
    double thresholdPotential;
    double resetPotential;
    double restingPotential;
    double membraneResistance;
    double membraneTimeConstant;
    bool spiked;

    std::unordered_set<Synapse*> outgoingSynapses;

    void spike();
};

#endif // NEURON_H
