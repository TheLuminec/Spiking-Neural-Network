#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <unordered_set>
#include "Neuron.h"
#include "Synapse.h"

class Network {
public:
    void addNeuron(Neuron* neuron);
    void addSynapse(Synapse* synapse);
    void simulate(double timeStep, double totalTime);
    void addInputNeuron(Neuron* neuron);
	void addOutputNeuron(Neuron* neuron);

private:
    std::unordered_set<Neuron*> neurons;
    std::unordered_set<Synapse*> synapses;
	std::vector<Neuron*> inputNeurons;
	std::vector<Neuron*> outputNeurons;
};

#endif // NETWORK_H
