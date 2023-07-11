#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(unsigned inputLayerSize, unsigned secondLayerSize, unsigned thirdLayerSize, unsigned outputLayerSize)
{
	//Initialize Layer values
	
	inputLayerValues = std::vector<double>(inputLayerSize);
	secondLayerValues = std::vector<double>(secondLayerSize);
	thirdLayerValues = std::vector<double>(thirdLayerSize);
	outputLayerValues = std::vector<double>(outputLayerSize);

	//Initialize Weights

	secondLayerWeights = std::vector<std::vector<double>>(secondLayerSize);
	thirdLayerWeights = std::vector<std::vector<double>>(thirdLayerSize);
	outputLayerWeights = std::vector<std::vector<double>>(outputLayerSize);

	for (auto& x : secondLayerWeights)
		x = std::vector<double>(inputLayerSize);
	for (auto& x : thirdLayerWeights)
		x = std::vector<double>(secondLayerSize);
	for (auto& x : outputLayerWeights)
		x = std::vector<double>(thirdLayerSize);
	
	//Initialize biases
	secondLayerBias = std::vector<double>(secondLayerSize);
	thirdLayerBias = std::vector<double>(thirdLayerSize);
	outputLayerBias = std::vector<double>(outputLayerSize);

}

void NeuralNetwork::loadInput(const std::vector<double>& input) {
	inputLayerValues = input;
}
void NeuralNetwork::launchNetwork()
{
	//Second layer
	for (unsigned i = 0; i < secondLayerValues.size(); i++) {
		double sum=0.0;
		for (unsigned j = 0; j < inputLayerValues.size(); j++) {
			sum += inputLayerValues[j] * secondLayerWeights[i][j];
		}
		secondLayerValues[i] = Utils::sigmoid(sum+secondLayerBias[i]);
	}
	//Third layer
	for (unsigned i = 0; i < thirdLayerValues.size(); i++) {
		double sum = 0.0;
		for (unsigned j = 0; j < secondLayerValues.size(); j++) {
			sum += secondLayerValues[j] * thirdLayerWeights[i][j];
		}

		thirdLayerValues[i] = Utils::sigmoid(sum + thirdLayerBias[i]);
	}
	//Output layer
	for (unsigned i = 0; i < outputLayerValues.size(); i++) {
		double sum = 0.0;
		for (unsigned j = 0; j < thirdLayerValues.size(); j++) {
			sum += thirdLayerValues[j] * outputLayerWeights[i][j];
		}

		outputLayerValues[i] = Utils::sigmoid(sum + outputLayerBias[i]);
	}
}

double NeuralNetwork::calculateCost(unsigned selected) {
	launchNetwork();
	double cost = 0;
	for (int i = 0; i < outputLayerValues.size(); i++)
	{
		cost += (outputLayerValues[i] - 0.0) * (outputLayerValues[i] - 0.0);
		if (i == selected) cost += (outputLayerValues[i] - 1.0) * (outputLayerValues[i] - 1.0);
	}
	return cost;
}

void NeuralNetwork::randomize()
{
	for (auto& w : secondLayerWeights)
		for (double& x : w) x = Utils::rand_double(-5.0, 5.0);

	for (double& x : secondLayerBias) {
		x = Utils::rand_double(-5.0, 5.0);
	}

	for (auto& w : thirdLayerWeights)
		for (double& x : w) x = Utils::rand_double(-5.0, 5.0);

	for (double& x : thirdLayerBias) {
		x = Utils::rand_double(-5.0, 5.0);
	}

	for (auto& w : outputLayerWeights)
		for (double& x : w) x = Utils::rand_double(-5.0, 5.0);

	for (double& x : outputLayerBias) {
		x = Utils::rand_double(-5.0, 5.0);
	}

}
