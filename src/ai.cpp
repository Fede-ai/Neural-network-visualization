#include "ai.h"
#include <iostream>

Ai::Ai(std::vector<int> size)
{
	for (int i = 1; i < size.size(); i++)
	{
		layers.push_back(Layer(size[i - 1], size[i]));
	}
}

void Ai::calculateOutput(std::vector<double> inputs)
{
	std::vector<double> computedLayer = layers[0].calculateLayer(inputs);

	for (int layer = 1; layer < layers.size(); layer++)
	{
		computedLayer = layers[layer].calculateLayer(computedLayer);
	}
}

void Ai::setRandomValues()
{	
	for (int layer = 0; layer < layers.size(); layer++)
	{
		layers[layer].setRandomLayerValues();
	}
}

double Ai::getOutput(int n) const
{
	return layers[layers.size() - 1].getNeuron(n);
}
double Ai::getNeuron(int layer, int neuron) const
{
	return layers[layer].getNeuron(neuron);
}
double Ai::getBias(int layer, int neuron) const
{
	return layers[layer].getBias(neuron);
}
double Ai::getWeight(int layer, int neuron, int neuronBefore) const
{
	return layers[layer].getWeight(neuron, neuronBefore);
}

void Ai::setBias(int layer, int neuron, double value)
{
	layers[layer].setBias(neuron, value);
}
void Ai::setWeight(int layer, int neuron, int neuronBefore, double value)
{
	layers[layer].setWeight(neuron, neuronBefore, value);
}