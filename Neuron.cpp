#include "Neuron.h"
#include "Synapse.h"

Neuron::Neuron(double threshold, double resetPot, double restPot, double memRes, double memTimeConst)
    : thresholdPotential(threshold),
    resetPotential(resetPot),
    restingPotential(restPot),
    membraneResistance(memRes),
    membraneTimeConstant(memTimeConst),
    membranePotential(restPot),
    spiked(false) {}

void Neuron::receiveCurrent(double current) {
    // Update membrane potential based on input current
    membranePotential += (current * membraneResistance);
}

void Neuron::update(double timeStep) {
    // Leaky Integrate-and-Fire model
    membranePotential += (-(membranePotential - restingPotential) / membraneTimeConstant) * timeStep;

    if (membranePotential >= thresholdPotential) {
        spike();
    }
    else {
        spiked = false;
    }
}

bool Neuron::hasSpiked() const {
    return spiked;
}

double Neuron::getPotential() const {
    return membranePotential;
}

void Neuron::spike() {
    spiked = true;
    membranePotential = resetPotential;

    // Notify all outgoing synapses
    for (auto& synapse : outgoingSynapses) {
        synapse->transmit();
    }
}

void Neuron::addOutgoingSynapse(Synapse* synapse) {
    outgoingSynapses.insert(synapse);
}
