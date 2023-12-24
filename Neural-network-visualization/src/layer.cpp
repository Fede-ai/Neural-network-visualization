#include "layer.h"
#include <random>

Layer::Layer(int inSizeBef, int inSizeAft)
{
	sizeAft = inSizeAft;
	sizeBef = inSizeBef;

	for (int a = 0; a < inSizeAft; a++)
	{
		weightedValues.push_back(0);
		activatedValues.push_back(0);
		biases.push_back(0);
		gradientsBiases.push_back(0);

		std::vector<double> neuronWeights;
		std::vector<double> neuronGradientsWeights;
		for (int b = 0; b < inSizeBef; b++)
		{
			neuronWeights.push_back(0);
			neuronGradientsWeights.push_back(0);
		}
		weights.push_back(neuronWeights);
		gradientsWeights.push_back(neuronGradientsWeights);
	}
	
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 
}

void Layer::setRandomLayerValues()
{
	for (int thisNeuron = 0; thisNeuron < activatedValues.size(); thisNeuron++)
	{
		for (int neuronBefore = 0; neuronBefore < weights[thisNeuron].size(); neuronBefore++)
		{
			weights[thisNeuron][neuronBefore] = (random(0, 2000) / 1000.f - 1);
		}
		biases[thisNeuron] = (random(0, 10000) / 1000.f - 5);
	}
}
int Layer::random(int min, int max)
{
	//std::random_device dev;
	//std::mt19937 rng(dev());
	//std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
	//return dist6(rng);

	return (rand() % (max - min + 1)) + min;
}
std::vector<double> Layer::calculateLayer(std::vector<double> nodesBef)
{
	lastLayerActivatedValues = nodesBef;
	for (int thisNeuron = 0; thisNeuron < activatedValues.size(); thisNeuron++)
	{
		double value = biases[thisNeuron];
		for (int neuronBefore = 0; neuronBefore < weights[thisNeuron].size(); neuronBefore++)
		{
			value += nodesBef[neuronBefore] * weights[thisNeuron][neuronBefore];
		}	
		weightedValues[thisNeuron] = value;
		activatedValues[thisNeuron] = activationFunction(value);
	}
	return activatedValues;
}

void Layer::updateGradients(std::vector<double> nodeValuesAft)
{
	for (int thisNeuron = 0; thisNeuron < activatedValues.size(); thisNeuron++)
	{	
		for (int neuronBefore = 0; neuronBefore < weights[thisNeuron].size(); neuronBefore++)
		{
			gradientsWeights[thisNeuron][neuronBefore] += lastLayerActivatedValues[neuronBefore] * nodeValuesAft[thisNeuron];
		}
		gradientsBiases[thisNeuron] += nodeValuesAft[thisNeuron];
	}
}
void Layer::applyGradients(double learnRate)
{
	for (int thisNeuron = 0; thisNeuron < activatedValues.size(); thisNeuron++)
	{
		biases[thisNeuron] -= gradientsBiases[thisNeuron] * learnRate;
		for (int neuronBefore = 0; neuronBefore < weights[thisNeuron].size(); neuronBefore++)
		{
			weights[thisNeuron][neuronBefore] -= gradientsWeights[thisNeuron][neuronBefore] * learnRate;
		}	
	}
}
void Layer::clearGradients()
{
	for (int thisNeuron = 0; thisNeuron < activatedValues.size(); thisNeuron++)
	{
		gradientsBiases[thisNeuron] = 0;
		for (int neuronBefore = 0; neuronBefore < weights[thisNeuron].size(); neuronBefore++)
		{
			gradientsWeights[thisNeuron][neuronBefore] = 0;
		}
	}
}

std::vector<double> Layer::calculateOutputLayerNodeValues(std::vector<double> expectedValue)
{
	std::vector<double> nodeValues;

	for (int i = 0; i < expectedValue.size(); i++)
	{
		double costToActivated = nodeCostDerivative(activatedValues[i], expectedValue[i]);
		double activatedToWeighted = activationFunctionDerivative(weightedValues[i]);
		nodeValues.push_back(costToActivated * activatedToWeighted);
	}	

	return nodeValues;
}
std::vector<double> Layer::calculateHiddenLayerNodeValues(std::vector<double> nodeValuesAft, std::vector<std::vector<double>> weightsAft)
{
	std::vector<double> newNodeValues;

	for (int node = 0; node < sizeAft; node++)
	{
		double newNodeValue = 0;
		for (int nodeAfter = 0; nodeAfter < weightsAft.size(); nodeAfter++)
		{
			newNodeValue += nodeValuesAft[nodeAfter] * weightsAft[nodeAfter][node];
		}
		newNodeValue *= activationFunctionDerivative(weightedValues[node]);
		newNodeValues.push_back(newNodeValue);
	}

	return newNodeValues;
}

double Layer::activationFunction(double weightedValue)
{
	return 1 / (1 + exp(-weightedValue));
}
double Layer::activationFunctionDerivative(double weightedValue)
{
	double activatedValue = activationFunction(weightedValue);
	return activatedValue * (1 - activatedValue);
}
double Layer::nodeCostDerivative(double activatedValue, double expectedValue)
{
	return 2 * (activatedValue - expectedValue);
}