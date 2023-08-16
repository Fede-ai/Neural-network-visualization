#pragma once
#include <vector>
#include <random>
#include "layer.h"

class Ai
{
public:
	Ai(std::vector<int> size);
	void calculateOutput(std::vector<double> inputs);
	void setRandomValues();

	double getOutput(int n) const;
	double getNeuron(int layer, int neuron) const;
	double getBias(int layer, int neuron) const;
	double getWeight(int layer, int neuron, int neuronBefore) const;

	void setBias(int layer, int neuron, double value);
	void setWeight(int layer, int neuron, int neuronBefore, double value);

private:
	std::vector<Layer> layers;
};