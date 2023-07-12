#pragma once

#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "../Other/Utils.h"
#include <iostream>
class NeuralNetwork
{
public:
	std::vector<double> inputLayerValues;
	std::vector<double> secondLayerValues;
	std::vector<double> thirdLayerValues;
	std::vector<double> outputLayerValues;

	std::vector<std::vector<double>> secondLayerWeights;
	std::vector<std::vector<double>> thirdLayerWeights;
	std::vector<std::vector<double>> outputLayerWeights;

	std::vector<double> secondLayerBias;
	std::vector<double> thirdLayerBias;
	std::vector<double> outputLayerBias;
	NeuralNetwork(unsigned inputLayerSize, unsigned secondLayerSize, unsigned thirdLayerSize, unsigned outputLayerSize);

	void loadInput(const std::vector<double>& input);

	void launchNetwork(); 

	double calculateCost(unsigned selected);

	void loadImage(const std::string& fileName);

	void loadWeightsAndBiases(const std::string& f1w, const std::string& f2w, const std::string& f3w, const std::string& f1b, const std::string& f2b, const std::string& f3b);

	void saveWeightsAndBiases(const std::string& f1w, const std::string& f2w, const std::string& f3w, const std::string& f1b, const std::string& f2b, const std::string& f3b);

	//randomizes weights and biases
	void randomize();


};