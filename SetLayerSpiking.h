#pragma once
/*
#ifndef SETLAYERSPIKING_H
#define SETLAYERSPIKING_H

#include "SpikerNetwork.h"

class SetLayerSpiking : public SpikerNetwork {
private:
	void (*outputFunc)(SpikerNetwork*, int);
	const int inputCount;
	const int outputCount;
	const int hiddenCount;
	const int hiddenSize;
	const int threshold;
	const int refractoryPeriod;

	void test() {

	}

	void randInit() {

		for (int i = 0; i < inputCount; i++) {
			createNeuronPair(i, 0, 0);

			for (int j = inputCount; j < hiddenCount + inputCount; j++) {
				neurons[i].first.addConnection(j);
			}
		}

		for (int i = 0; i < hiddenSize - 1; i++) {
			for (int j = inputCount + (i * hiddenCount); j < inputCount + ((i+1) * hiddenCount); j++) {
				createNeuronPair(j, threshold, refractoryPeriod);

				for (int k = inputCount + ((i+1) * hiddenCount); k < inputCount + ((i + 2) * hiddenCount); k++) {
					neurons[j].first.addConnection(k);
				}
			}
		}

		for(int i = inputCount + (hiddenCount * (hiddenSize - 1)); i < inputCount + (hiddenSize * hiddenCount); i++) {
			createNeuronPair(i, threshold, refractoryPeriod);

			for (int j = inputCount + (hiddenSize * hiddenCount); j < inputCount + (hiddenSize * hiddenCount) + outputCount; j++) {
				createNeuronPair(j, threshold, refractoryPeriod, j - (inputCount + (hiddenSize * hiddenCount)), outputFunc);
				neurons[i].first.addConnection(j);
			}
		}

	}
	

public:
	SetLayerSpiking() = default;

	explicit SetLayerSpiking(int inputCount, int outputCount, int hiddenCount, int hiddenSize, int threshold, int refractoryPeriod, void (*func)(SpikerNetwork*, int)) : SpikerNetwork(),
		inputCount(inputCount), outputCount(outputCount), hiddenCount(hiddenCount), hiddenSize(hiddenSize), 
		threshold(threshold), refractoryPeriod(refractoryPeriod), outputFunc(func)
	{
		randInit();
	}

	void init() {
		reserve();

	}

	void spikeInput(int id) {

		queueNeuron(id);
	}

};


#endif // !SETLAYERSPIKING_H
*/