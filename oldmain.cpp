/*
#include <iostream>
#include <string>
#include "SetLayerSpiking.h"
#include "SpikerExporter.h"

void selfInputLoop(SpikerNetwork &nn) {
    int input = 0;
    while (input != -1) {
        std::string sInput;
        std::cin >> sInput;

        try {
            input = std::stoi(sInput);
        }
        catch (std::invalid_argument) {
            continue;
        }

        std::cout << "Inputing: " << input << std::endl;
        nn.queueNeuron(input);
    }
}

int main() {
    std::cout << "Working." << std::endl;
    //SpikerNetwork nn = SpikerExporter::loadNetworkJSON("networks/network.json");
    SetLayerSpiking nn(10, 10, 20, 3, 7, 2, &output);
    
    //nn.outputSet(&output);
    nn.reserve();


    nn.startLoop(60);


	selfInputLoop(nn);
    
  
	nn.stopLoop();
    std::cout << "Stopped." << std::endl;

    SpikerExporter::saveNetworkJSON(nn, "networks/network.json");

    return 0;
}

void output(SpikerNetwork *nn, int id) {
    std::cout << "Output: " << id << std::endl;
    if (id == 9)
        nn->applyReward(1.0);
    else
        nn->applyReward(-1.0);
}
*/