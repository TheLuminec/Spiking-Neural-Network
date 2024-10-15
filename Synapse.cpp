#include "Synapse.h"
#include "Neuron.h"

Synapse::Synapse(Neuron* pre, Neuron* post, double weight)
    : preNeuron(pre), postNeuron(post), synapticWeight(weight) {
    // Register this synapse with the pre-synaptic neuron
    preNeuron->addOutgoingSynapse(this);
}

void Synapse::transmit() {
    // Directly transmit the current to the post-synaptic neuron
    postNeuron->receiveCurrent(synapticWeight);
}

double Synapse::getWeight() const {
    return synapticWeight;
}

void Synapse::setWeight(double newWeight) {
    synapticWeight = newWeight;
}
