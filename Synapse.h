#ifndef SYNAPSE_H
#define SYNAPSE_H

class Neuron; // Forward declaration

class Synapse {
public:
    Synapse(Neuron* preSynapticNeuron, Neuron* postSynapticNeuron, double weight);

    void transmit();
    double getWeight() const;
    void setWeight(double newWeight);

private:
    Neuron* preNeuron;
    Neuron* postNeuron;
    double synapticWeight;
};

#endif // SYNAPSE_H
