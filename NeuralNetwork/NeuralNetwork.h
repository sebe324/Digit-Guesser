#pragma once

#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "../Other/Utils.h"
#include <iostream>
#include <stdio.h>
#include "Neuron.h"
#include <cassert>

typedef std::vector<Neuron> Layer;

class NeuralNetwork
{
public:


	NeuralNetwork(const std::vector<unsigned>& topology);

	void feedForward(const std::vector<double>& inputValues);
	void backpropagate(const std::vector<double>& targetValues);
	void getResults(std::vector<double>& resultVals) const;

	void loadWeights(const std::vector<std::string>& fileNames);
	void saveWeights(const std::vector<std::string>& fileNames) const;
	void createLearningBatch(const std::string& fileName, const std::string& labelFileName);
	std::vector<std::vector<unsigned char>> loadTrainingData(const std::string& fileName, unsigned amount);
	std::vector<std::vector<double>> getTargetValues(const std::string& labelFileName, unsigned amount);

	double m_recentAverageError;
	double m_error;
private:

	std::vector<Layer> m_layers;
	Layer& m_inputLayer;
	Layer& m_outputLayer;
	double m_recentAverageSmoothingFactor;
	double calculateRMS(const std::vector<double>& targetVals) const;
};