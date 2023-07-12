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

void NeuralNetwork::loadWeightsAndBiases
(const std::string& f1w, const std::string& f2w, const std::string& f3w, const std::string& f1b, const std::string& f2b, const std::string& f3b)
{

}

void NeuralNetwork::saveWeightsAndBiases
(const std::string& f1w, const std::string& f2w, const std::string& f3w, const std::string& f1b, const std::string& f2b, const std::string& f3b)
{
	std::ofstream file1Weights(f1w);
	std::ofstream file2Weights(f2w);
	std::ofstream file3Weights(f3w);

	std::ofstream file1Biases(f1b);
	std::ofstream file2Biases(f2b);
	std::ofstream file3Biases(f3b);

	for (int i = 0; i < secondLayerValues.size(); i++) {
		for (int j = 0; j < inputLayerValues.size(); j++) {
			file1Weights << std::to_string(secondLayerWeights[i][j])<<",";
		}
		file1Weights << "\n";
		file1Biases << std::to_string(secondLayerBias[i]) << ",";
	}

	for (int i = 0; i < thirdLayerValues.size(); i++) {
		for (int j = 0; j < secondLayerValues.size(); j++) {
			file2Weights << std::to_string(thirdLayerWeights[i][j]) << ",";
		}
		file2Weights << "\n";
		file2Biases << std::to_string(thirdLayerBias[i]) << ",";
	}

	for (int i = 0; i < outputLayerValues.size(); i++) {
		for (int j = 0; j < thirdLayerValues.size(); j++) {
			file3Weights << std::to_string(outputLayerWeights[i][j]) << ",";
		}
		file3Weights << "\n";
		file3Biases << std::to_string(outputLayerBias[i]) << ",";
	}
	file1Weights.close();
	file2Weights.close();
	file3Weights.close();

	file1Biases.close();
	file2Biases.close();
	file3Biases.close();
}