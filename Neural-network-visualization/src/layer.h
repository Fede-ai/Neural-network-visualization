#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Layer
{
public:
	//input layer is not a layer
	//layer obj contains the nodes and the wheights between the nodes of this layer and the previus one
	Layer(int inSizeBef, int inSizeAft);

	void setRandomLayerValues();
	static int random(int min, int max);
	std::vector<double> calculateLayer(std::vector<double> nodesBef);

	void updateGradients(std::vector<double> nodeValuesAft);
	void applyGradients(double learnRate);
	void clearGradients();

	std::vector<double> calculateOutputLayerNodeValues(std::vector<double> expectedValue);
	std::vector<double> calculateHiddenLayerNodeValues(std::vector<double> nodeValuesAft, std::vector<std::vector<double>> weightsAft);

	//the number of nodes in the layer
	int sizeAft;
	int sizeBef;

	//the weighted values after passing through the activation function
	std::vector<double> activatedValues;
	std::vector<double> biases;
	//how to access correct weight -> weights[nodeAft][nodeBef]
	std::vector<std::vector<double>> weights;

private:	
	//the activated values that come out of the layer before | for backpropagation
	std::vector<double> lastLayerActivatedValues;
	//the activated values before passing through the activation function | for backpropagation
	std::vector<double> weightedValues;

	//for backpropagation
	std::vector<double> gradientsBiases;
	//for backpropagation
	std::vector<std::vector<double>> gradientsWeights;

	double activationFunction(double weightedValue);
	double activationFunctionDerivative(double weightedValue);
	double nodeCostDerivative(double activatedValue, double expectedValue);
};