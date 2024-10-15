#include "Network.h"
#include <iostream>

void Network::addNeuron(Neuron* neuron) {
    neurons.insert(neuron);
}

void Network::addSynapse(Synapse* synapse) {
    synapses.insert(synapse);
}

void Network::addInputNeuron(Neuron* neuron) {
	inputNeurons.push_back(neuron);
    addNeuron(neuron);
}

void Network::addOutputNeuron(Neuron* neuron) {
	outputNeurons.push_back(neuron);
    addNeuron(neuron);
}

void Network::simulate(double timeStep, double totalTime) {
    int steps = static_cast<int>(totalTime / timeStep);
    for (int i = 0; i < steps; ++i) {
        inputNeurons[0]->receiveCurrent(1.0);

        // Update neurons
        for (auto& neuron : neurons) {
            neuron->update(timeStep);
        }

        for (auto& n : outputNeurons) {
            if (n->hasSpiked()) {
                std::cout << "Spike at time " << i * timeStep << " ms" << std::endl;
            }
        }
        
    }
}
