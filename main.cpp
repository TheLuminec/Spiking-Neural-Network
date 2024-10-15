#include <iostream>
#include "Network.h"

int main() {
    // Create network
    Network network;

    // Create neurons
    Neuron* neuron1 = new Neuron(5.0, 0.0, 1.0, 8.0, 4.0);
    Neuron* neuron2 = new Neuron(5.0, 0.0, 1.0, 4.0, 4.0);

    // Add neurons to network
    network.addInputNeuron(neuron1);
    network.addOutputNeuron(neuron2);

    // Create synapse with weight
    Synapse* synapse = new Synapse(neuron1, neuron2, 0.5);

    // Add synapse to network
    network.addSynapse(synapse);

    // Simulate network for 100 ms with a time step of 1 ms
    network.simulate(1.0, 100.0);

    // Clean up
    delete neuron1;
    delete neuron2;
    delete synapse;

    return 0;
}
