#pragma once
#include <vector>
#include <random>
#include <fstream>
#include "layer.h"
#include "dataPoint.h"
#include <iostream>
#include <windows.h>

class Ai
{
public:	
	//init empty ai from size
	Ai(std::vector<int> inSize);
	//init empty ai from console 
	Ai(int sizeIn, int sizeOut);
	//init ai from file path
	Ai(std::string path, bool& failed);
	//init ai from user-chosen file path
	Ai(bool& failed);

	void setRandomValues();
	std::vector<double> calculateOutput(std::vector<double> inputs);

	void learn(std::vector<DataPoint> trainingData, double learnRate);
	double cost(std::vector<DataPoint> datapoints);
	
	//including the input layer
	std::vector<int> getSize() const;
	double getNode(int layer, int node) const;
	double getBias(int layer, int node) const;
	double getWeight(int layer, int nodeAft, int nodeBef) const;

	void setBias(int layer, int node, double value);
	void setWeight(int layer, int nodeAft, int nodeBef, double value);

	//returns false if fails
	bool save(std::string filePath) const;
	//returns false if fails
	bool save() const;

private:	
	std::vector<Layer> layers;
	//including the input layer
	std::vector<int> sizeAi;
};