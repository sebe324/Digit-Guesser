#include "Neuron.h"


Connection::Connection()
{
	weight = Utils::rand_double(-0.5, 0.5);
	deltaWeight = 0.0;
}

double Neuron::eta = 0.01;
double Neuron::alpha = 0.5;

Neuron::Neuron(unsigned numOutputs, unsigned index) {
	for (unsigned c = 0; c < numOutputs; c++) 
		m_outputWeights.push_back(Connection());

	m_index = index;
	m_gradient = 0.0;
	m_outputVal = 0.0;
}

void Neuron::feedForward(Layer& prevLayer) {
	double sum = 0.0;
	for (unsigned n = 0; n < prevLayer.size(); n++) {
		sum += prevLayer[n].m_outputVal * prevLayer[n].m_outputWeights[m_index].weight;
	}
	m_outputVal = transferFunction(sum);
}

void Neuron::calcOutputGradients(double targetVal) {
	double delta = targetVal - m_outputVal;
	m_gradient = delta * transferFunctionDerivative(m_outputVal);
}

void Neuron::calcHiddenGradients(const Layer & nextLayer) {
	double dow = sumDOW(nextLayer);
	m_gradient = dow * transferFunctionDerivative(m_outputVal);
}

double Neuron::sumDOW(const Layer& nextLayer) {
	double sum = 0.0;

	for (unsigned n = 0; n < nextLayer.size() - 1; n++) {
		sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
	}
	return sum;
}

void Neuron::updateInputWeights(Layer& prevLayer) {
	for (unsigned n = 0; n < prevLayer.size(); n++) {
		Neuron& neuron = prevLayer[n];
		double oldDeltaWeight = neuron.m_outputWeights[m_index].deltaWeight;
		double newDeltaWeight = eta * neuron.m_outputVal * m_gradient + alpha * oldDeltaWeight;
		neuron.m_outputWeights[m_index].deltaWeight = newDeltaWeight;
		neuron.m_outputWeights[m_index].weight += newDeltaWeight;
	}
}

double Neuron::transferFunction(double v) {
	return (v / (1 + abs(v)) + 1) / 2;
}

double Neuron::transferFunctionDerivative(double v) {
	return 2 / ((2 + 2 * abs(v)) * (2 + 2 * abs(v)));
}