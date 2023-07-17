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
	std::ifstream file1Weights(f1w);
	std::ifstream file2Weights(f2w);
	std::ifstream file3Weights(f3w);

	std::ifstream file1Biases(f1b);
	std::ifstream file2Biases(f2b);
	std::ifstream file3Biases(f3b);

	std::string line;
	int i = 0;
	int j = 0;

	int count = 0;
	//Load weights
	for (int i = 0; i < secondLayerValues.size(); i++) {
		std::getline(file1Weights, line);
		std::vector<std::string> temp=Utils::split(line,',');
		for (int j = 0; j < inputLayerValues.size(); j++) {
			secondLayerWeights[i][j]=std::stod(temp[j]);
		}
	}
	for (int i = 0; i < thirdLayerValues.size(); i++) {
		std::getline(file2Weights, line);
		std::vector<std::string> temp = Utils::split(line, ',');
		for (int j = 0; j < secondLayerValues.size(); j++) {
			thirdLayerWeights[i][j] = std::stod(temp[j]);
		}
	}
	for (int i = 0; i < outputLayerValues.size(); i++) {
		std::getline(file3Weights, line);
		std::vector<std::string> temp = Utils::split(line, ',');
		for (int j = 0; j < thirdLayerValues.size(); j++) {
			outputLayerWeights[i][j] = std::stod(temp[j]);
		}
	}
	//load biases
	i = 0;
	while (std::getline(file1Biases, line, ',')) secondLayerBias[i++] = std::stod(line);
	i = 0;
	while (std::getline(file2Biases, line, ',')) thirdLayerBias[i++] = std::stod(line);
	i = 0;
	while (std::getline(file3Biases, line, ',')) outputLayerBias[i++] = std::stod(line);
	
	
	file1Weights.close();
	file2Weights.close();
	file3Weights.close();

	file1Biases.close();
	file2Biases.close();
	file3Biases.close();
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

	int count = 0;

	for (int i = 0; i < secondLayerValues.size(); i++) {
		for (int j = 0; j < inputLayerValues.size(); j++) {
			file1Weights << std::to_string(secondLayerWeights[i][j]);
			if (j < inputLayerValues.size() - 1) file1Weights<<",";
			count++;
		}
		file1Weights << "\n";
		file1Biases << std::to_string(secondLayerBias[i]);
		if (i < secondLayerValues.size() - 1) file1Biases << ",";
	}

	for (int i = 0; i < thirdLayerValues.size(); i++) {
		for (int j = 0; j < secondLayerValues.size(); j++) {
			file2Weights << std::to_string(thirdLayerWeights[i][j]);
			if (j < secondLayerValues.size() - 1) file2Weights << ",";
		}
		file2Weights << "\n";
		file2Biases << std::to_string(thirdLayerBias[i]);
		if (i < thirdLayerValues.size() - 1) file2Biases << ",";
	}


	for (int i = 0; i < outputLayerValues.size(); i++) {
		for (int j = 0; j < thirdLayerValues.size(); j++) {
			file3Weights << std::to_string(outputLayerWeights[i][j]);
			if (j < thirdLayerValues.size() - 1) file3Weights << ",";
		}
		file3Weights << "\n";
		file3Biases << std::to_string(outputLayerBias[i]);
		if (i < outputLayerValues.size() - 1) file3Biases << ",";
	}
	file1Weights.close();
	file2Weights.close();
	file3Weights.close();

	file1Biases.close();
	file2Biases.close();
	file3Biases.close();
}

std::vector<unsigned char> NeuralNetwork::loadData(const std::string& fileName)
{
	std::ifstream input(fileName, std::ios::binary);
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
	input.close();
	std::cout << "MADE IT" << std::endl;
	return buffer;
}

void NeuralNetwork::createLearningBatches()
{
	std::vector<std::vector<unsigned char>> v(10);
	v[0] = loadData("TrainingData\\data0");
	v[1] = loadData("TrainingData\\data1");
	v[2] = loadData("TrainingData\\data2");
	v[3] = loadData("TrainingData\\data3");
	v[4] = loadData("TrainingData\\data4");
	v[5] = loadData("TrainingData\\data5");
	v[6] = loadData("TrainingData\\data6");
	v[7] = loadData("TrainingData\\data7");
	v[8] = loadData("TrainingData\\data8");
	v[9] = loadData("TrainingData\\data9");
	for (int i = 0; i < 100; i++)
	{
		std::ofstream batch("LearningBatches\\batch" + std::to_string(i), std::ios::binary);
		std::ofstream label("LearningBatches\\label" + std::to_string(i), std::ios::binary);
		std::vector<unsigned char> tmp(100*784);
		for (int j = 0; j < 10; j++) {
			for (unsigned char k = 0; k < 10; k++) {
				memcpy(tmp.data() + j * 10 * 784 + k * 784, v[k].data() + i * 784 * 10 + j * 784, 784);
				label.write(reinterpret_cast<char*>(&k), sizeof(k));
			}
		}
		batch.write(reinterpret_cast<char*>(&tmp[0]), sizeof(unsigned char) * tmp.size());
		batch.close();
		label.close();
	}
}