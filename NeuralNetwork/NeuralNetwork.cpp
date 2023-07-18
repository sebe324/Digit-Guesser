#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(const std::vector<unsigned>& topology)
	: m_layers(topology.size()), m_inputLayer(m_layers.front()), m_outputLayer(m_layers.back())
{
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; layerNum++) {
		m_layers[layerNum] = Layer();

		//The last layer has no outputs.
		unsigned numOutputs =
		layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		// <= is used instead of < because of the bias neuron.
		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; neuronNum++) {
			m_layers[layerNum].push_back(Neuron(numOutputs, neuronNum));
		}

		//set the bias neuron to 1.0 in each layer.
		m_layers[layerNum].back().m_outputVal = 1.0;
	}


	m_error = 0.0;
	m_recentAverageError=0.0;
	m_recentAverageSmoothingFactor=0.0;
}

void NeuralNetwork::feedForward(const std::vector<double>& inputValues) {

	assert(inputValues.size() == m_inputLayer.size() - 1);
	for (unsigned i = 0; i < inputValues.size(); i++)
		m_inputLayer[i].m_outputVal = inputValues[i];
	
	for (unsigned layerNum = 1; layerNum < m_layers.size(); layerNum++) {
		Layer& prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < m_layers[layerNum].size() - 1; n++) {
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}

void NeuralNetwork::backpropagate(const std::vector<double>& targetValues) {

	//Calculate error (cost) of the neural network
	m_error = calculateRMS(targetValues);

	//Recent average measurment

	m_recentAverageError =
		(m_recentAverageError * m_recentAverageSmoothingFactor + m_error) / (m_recentAverageSmoothingFactor + 1.0);
	
	//Output layer gradients
	for (unsigned n = 0; n < m_outputLayer.size() - 1; n++) {
		m_outputLayer[n].calcOutputGradients(targetValues[n]);
	}

	//Hidden layers gradients

	for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; layerNum--) {
		Layer& hiddenLayer = m_layers[layerNum];
		Layer& nextLayer = m_layers[layerNum + 1];

		for (unsigned n = 0; n < hiddenLayer.size(); n++) {
			hiddenLayer[n].calcHiddenGradients(nextLayer);
		}
	}

	//Update connection weights

	for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; layerNum--) {
		Layer& layer = m_layers[layerNum];
		Layer& prevLayer = m_layers[layerNum - 1];

		for (unsigned n = 0; n < layer.size() - 1; n++) {
			layer[n].updateInputWeights(prevLayer);
		}
	}
}

void NeuralNetwork::getResults(std::vector<double>& resultVals) const {
	resultVals.clear();
	for (unsigned n = 0; n < m_outputLayer.size() - 1; n++) {
		resultVals.push_back(m_outputLayer[n].m_outputVal);
	}
}

void NeuralNetwork::loadWeights(const std::vector<std::string>& fileNames) {
	// fileNames size should be the same as m_layers size - 1, becasue last layer has no outputs.
	assert(fileNames.size() == m_layers.size() - 1);

	for (unsigned i = 0; i < fileNames.size(); i++) {
		std::ifstream file(fileNames[i]);

		for (unsigned j = 0; j < m_layers[i].size(); j++) {
			std::string line;
			std::getline(file, line);
			std::vector<std::string> values_split=Utils::split(line,';');

			for (unsigned c = 0; c < values_split.size(); c++) {
				m_layers[i][j].m_outputWeights[c].weight = std::stod(values_split[c]);
			}
		}
		file.close();
	}
}

void NeuralNetwork::saveWeights(const std::vector<std::string>& fileNames) const{
	// fileNames size should be the same as m_layers size - 1, becasue last layer has no outputs.
	assert(fileNames.size() == m_layers.size() - 1);
	for (unsigned i = 0; i < fileNames.size(); i++) {

		std::ofstream file(fileNames[i]);

		for (unsigned j = 0; j < m_layers[i].size(); j++) {
			// - 2, because there is no connection to the bias.
			for (unsigned c = 0; c < m_layers[i + 1].size()-2; c++) {
				file<<m_layers[i][j].m_outputWeights[c].weight<<";";
			}
			file << m_layers[i][j].m_outputWeights[m_layers[i + 1].size() - 2].weight;
			file << '\n';
		}

		file.close();
	}
}


double NeuralNetwork::calculateRMS(const std::vector<double>& targetVals) const {
	double result = 0.0;
	for (int i = 0; i < m_outputLayer.size() - 1; i++) {
		double delta = m_outputLayer[i].m_outputVal - targetVals[i];
		result += delta * delta;
	}
	result /= m_outputLayer.size() - 1;
	result = sqrt(result);
	return result;
}

std::vector<std::vector<unsigned char>> NeuralNetwork::loadTrainingData(const std::string& fileName, unsigned amount) {
	std::ifstream input(fileName, std::ios::binary);
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
	std::cout << buffer.size() << std::endl;
	std::cout << m_inputLayer.size() << std::endl;
	std::vector<std::vector<unsigned char>> result(amount,std::vector<unsigned char>(m_inputLayer.size()-1));
	for (int i = 0; i < amount; i++) {
		memcpy(result[i].data(), buffer.data() + i * (m_inputLayer.size()-1), m_inputLayer.size() - 1);
	}
	input.close();
	return result;
}

void NeuralNetwork::createLearningBatch(const std::string& fileName, const std::string& labelFileName) {
	//i know this line is ugly xd
	std::vector<std::vector<std::vector<unsigned char>>> v(10, std::vector<std::vector<unsigned char>>(1000,std::vector<unsigned char>(784)));
	for (int i = 0; i < 10; i++) v[i] = loadTrainingData(std::string("TrainingData\\data") + std::to_string(i), 1000);

	std::ofstream dataFile(fileName, std::ios::binary);
	dataFile.clear();
	std::ofstream labelFile(labelFileName, std::ios::binary);
	labelFile.clear();
	std::vector<unsigned char> tmp(10000 * 784);
	for (int i = 0; i < 1000; i++)
	{
		std::cout << "completed " << i << std::endl;
		for (int j = 0; j < 10; j++) {
			memcpy(tmp.data()+i*10*784+j*784, v[j][i].data(), 784);
			labelFile.write(reinterpret_cast<char*>(&j), sizeof(unsigned char));
		}
	}
	dataFile.write(reinterpret_cast<char*>(&tmp[0]), sizeof(unsigned char) * tmp.size());
	dataFile.close();

}

std::vector<std::vector<double>> NeuralNetwork::getTargetValues(const std::string& labelFileName, unsigned amount) {
	std::ifstream input(labelFileName, std::ios::binary);
	std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
	std::cout << "Buffer size:" << buffer.size() << std::endl;
	std::vector<std::vector<double>> result(amount,std::vector<double>(10,0.0));
	for (int i = 0; i < amount; i++) {
		//std::cout << "iteration: " << i << "buffer value: " << (int)buffer[i] << std::endl;
		result[i][buffer[i]] = 1.0;
	}
	input.close();
	return result;
}