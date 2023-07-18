#pragma once

#include <vector>
#include <iostream>

#include "../Other/Utils.h"

class Neuron;

typedef std::vector<Neuron> Layer;

class Connection
{
public:

	double weight;
	double deltaWeight;

	Connection();
};

class Neuron {
public:
	Neuron(unsigned numOutputs, unsigned index);
	void feedForward(Layer& prevLayer);
	void calcOutputGradients(double targetVal);
	void calcHiddenGradients(const Layer& nextLayer);
	double sumDOW(const Layer& nextLayer);
	void updateInputWeights(Layer& prevLayer);

	double m_outputVal;
	std::vector<Connection> m_outputWeights;

private:
	double m_gradient;
	unsigned m_index;
	static double eta;
	static double alpha;

	double transferFunction(double v);
	double transferFunctionDerivative(double v);
};
