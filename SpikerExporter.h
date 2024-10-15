#pragma once
/*
#ifndef SPIKEREXPORTER_H
#define SPIKEREXPORTER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <utility>
#include "SpikerNetwork.h"
#include "json.hpp" // Include the nlohmann/json library

using json = nlohmann::json;

class SpikerExporter {
public:
    static void saveNetworkJSON(const SpikerNetwork& nn, const std::string& filename) {
        auto [neurons, connections] = nn.returnNetwork();

        json j;

        // Serialize neurons
        for (const auto& neuron : neurons) {
            json neuronJson;
            neuronJson["id"] = neuron.id;
            neuronJson["threshold"] = neuron.threshold;
            neuronJson["refractoryPeriod"] = neuron.refractoryPeriod;
            neuronJson["funcOutput"] = neuron.funcOutput;
            neuronJson["funcId"] = neuron.funcId;
            neuronJson["paths"] = json::array();

            for (const auto& connection : neuron.paths) {
                json connectionJson;
                connectionJson["from"] = connection.from;
                connectionJson["to"] = connection.to;
                connectionJson["weight"] = connection.weight;

                neuronJson["paths"].push_back(connectionJson);
            }

            j["neurons"].push_back(neuronJson);
        }

        // Write JSON to file
        std::ofstream file(filename);
        if (file.is_open()) {
            file << j.dump(4); // Pretty-print with an indent of 4 spaces
            file.close();
        }
        else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }
    }

    static SpikerNetwork loadNetworkJSON(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            throw std::runtime_error("Unable to open file");
        }

        json j;
        file >> j;
        file.close();

		std::set<Neuron> neurons;

        // Deserialize neurons
        for (const auto& neuronJson : j["neurons"]) {
            int id = neuronJson["id"];
            int threshold = neuronJson["threshold"];
            int refractoryPeriod = neuronJson["refractoryPeriod"];
            bool funcOutput = neuronJson["funcOutput"];
            int funcId = neuronJson["funcId"];

            Neuron neuron(id, threshold, refractoryPeriod);
            neuron.funcOutput = funcOutput;
            neuron.funcId = funcId;

            for (const auto& connectionJson : neuronJson["paths"]) {
                int to = connectionJson["to"];
                double weight = connectionJson["weight"];

                neuron.addConnection(to, weight);
            }

			neurons.insert(neuron);
        }

        return SpikerNetwork(neurons);
    }
};

#endif //!SPIKEREXPORTER_H
*/