#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Layer
{
public:
	//input layer is not a layer
	//layer obj contains the nodes and the wheights between the nodes of this layer and the previus one
	Layer(int sizeBefore, int size);
	std::vector<double> calculateLayer(std::vector<double> neuronsBefore);
	void setRandomLayerValues();
	
	static int random(int min, int max);

	double getNeuron(int neuron) const;
	double getBias(int neuron) const;
	double getWeight(int neuron, int neuronBefore) const;

	void setBias(int neuron, double value);
	void setWeight(int neuron, int neuronBefore, double value);

private:
	double activationFunction(double num);	
	
	std::vector<double> neurons;
	std::vector<double> biases;	
	//how to access correct weight -> weights[thisNeuron][neuronBefore]
	std::vector<std::vector<double>> weights;
};