#include "ai.h"

Ai::Ai(std::vector<int> inSize)
{
	sizeAi = inSize;
	for (int i = 1; i < inSize.size(); i++)
	{
		layers.push_back(Layer(inSize[i - 1], inSize[i]));
	}
}
Ai::Ai(int sizeIn, int sizeOut)
{
	std::string cmd;
	std::vector<int> aiSize;
	aiSize.push_back(sizeIn);
	do {
		std::cout << "enter the desired network size, layer by layer\n";
		std::cout << "(enter \'start\' to continue with the selected size)\n";
		std::cout << "current network's size: ";
		for (int i = 0; i < aiSize.size(); i++)
		{
			std::cout << aiSize[i] << ", ";
		}
		std::cout << "_. next layer's size: ";

		std::cin >> cmd;
		std::string::const_iterator it = cmd.begin();
		while (it != cmd.end() && std::isdigit(*it)) ++it;
		if (!cmd.empty() && it == cmd.end() && stoi(cmd) != 0)
		{
			aiSize.push_back(stoi(cmd));
		}

		std::cout << "\n";
	} while (cmd != "start");

	std::cout << "final network's size: ";
	for (int i = 0; i < aiSize.size(); i++)
	{
		std::cout << aiSize[i] << ", ";
	}
	aiSize.push_back(sizeOut);
	std::cout << "2.";

	*this = Ai(aiSize);
}
Ai::Ai(std::string path, bool& failed)
{
	std::fstream file;
	file.open(path, std::ios::in);

	if (!file.good())
	{
		failed = true;
	}
	else
	{
		failed = false;
		char div;
		//extract layers size
		do {
			int value;
			file >> value;
			sizeAi.push_back(value);
			file >> div;
		} while (div == ',');
		//load layers size
		for (int i = 1; i < sizeAi.size(); i++)
		{
			layers.push_back(Layer(sizeAi[i - 1], sizeAi[i]));
		}
		//load biases
		for (int layer = 0; layer < layers.size(); layer++)
		{
			for (int bias = 0; bias < sizeAi[layer + 1]; bias++)
			{
				double value;
				file >> value;
				layers[layer].biases[bias] = value;
				file >> div;
			}
		}
		//load weights
		for (int layer = 0; layer < layers.size(); layer++)
		{
			for (int nodeAft = 0; nodeAft < layers[layer].sizeAft; nodeAft++)
			{
				for (int nodeBef = 0; nodeBef < layers[layer].sizeBef; nodeBef++)
				{
					double value;
					file >> value;
					layers[layer].weights[nodeAft][nodeBef] = value;
					file >> div;
				}
			}
		}
		file.close();
	}	
}
Ai::Ai(bool& failed)
{	
	OPENFILENAMEA file;
	char path[100];

	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = NULL;
	file.lpstrFile = path;
	file.lpstrFile[0] = '\0';
	file.nMaxFile = sizeof(path);
	file.lpstrFilter = (PSTR)"Text (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	file.nFilterIndex = 1;
	file.lpstrFileTitle = NULL;
	file.nMaxFileTitle = 0;
	file.lpstrInitialDir = NULL;
	file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&file) == false)
	{
		failed = true;
	}
	else
	{
		*this = Ai((std::string)path, failed);
	}
}

void Ai::setRandomValues()
{	
	for (int layer = 0; layer < layers.size(); layer++)
	{
		layers[layer].setRandomLayerValues();
	}
}
std::vector<double> Ai::calculateOutput(std::vector<double> inputs)
{
	if (inputs.size() != layers[0].sizeBef)
		std::exit(1);

	std::vector<double> computedLayer = layers[0].calculateLayer(inputs);

	for (int layer = 1; layer < layers.size(); layer++)
	{
		computedLayer = layers[layer].calculateLayer(computedLayer);
	}

	return computedLayer;
}

void Ai::learn(std::vector<DataPoint> trainingData, double learnRate)
{	
	for (auto datapoint : trainingData)
	{	
		calculateOutput(datapoint.inputs);
		std::vector<double> nodeValues = layers[layers.size()-1].calculateOutputLayerNodeValues(datapoint.targets);
		layers[layers.size()-1].updateGradients(nodeValues);
		for (int layer = layers.size() - 2; layer >= 0; layer--)
		{
			nodeValues = layers[layer].calculateHiddenLayerNodeValues(nodeValues, layers[layer + 1].weights);
			layers[layer].updateGradients(nodeValues);
		}
	}
	for (auto& layer : layers)
	{
		layer.applyGradients(learnRate / trainingData.size());
		layer.clearGradients();
	}
}
double Ai::cost(std::vector<DataPoint> datapoints)
{
	double cost = 0;
	for (auto datapoint : datapoints)
	{
		std::vector<double> outputs = calculateOutput(datapoint.inputs);
		for (int output = 0; output < outputs.size(); output++)
		{
			cost += std::pow(outputs[output] - datapoint.targets[output], 2);
		}
	}
	return cost / datapoints.size();
}

std::vector<int> Ai::getSize() const
{
	return sizeAi;
}
double Ai::getNode(int layer, int node) const
{
	return layers[layer].activatedValues[node];
}
double Ai::getBias(int layer, int node) const
{
	return layers[layer].biases[node];
}
double Ai::getWeight(int layer, int nodeAft, int nodeBef) const
{
	return layers[layer].weights[nodeAft][nodeBef];
}

void Ai::setBias(int layer, int node, double value)
{
	layers[layer].biases[node] = value;
}
void Ai::setWeight(int layer, int nodeAft, int nodeBef, double value)
{
	layers[layer].weights[nodeAft][nodeBef] = value;
}

bool Ai::save(std::string filePath) const
{
	std::fstream file;
	file.open(filePath, std::ios::out | std::ios::trunc);

	//fails
	if (!file.is_open())
		return false;

	for (int layer = 0; layer < sizeAi.size(); layer++)
	{
		file <<	sizeAi[layer];
		if (layer == sizeAi.size() - 1)
			file << ';';
		else
			file << ',';
	}
	file << '\n';
	
	for (int layer = 0; layer < layers.size(); layer++)
	{
		for (int bias = 0; bias < layers[layer].biases.size(); bias++)
		{
			file << layers[layer].biases[bias];
			if (bias == layers[layer].biases.size() - 1)
				file << ';';
			else
				file << ',';
		}
		file << '\n';
	}
	
	for (int layer = 0; layer < layers.size(); layer++)
	{
		for (int nodeAft = 0; nodeAft < layers[layer].sizeAft; nodeAft++)
		{
			for (int nodeBef = 0; nodeBef < layers[layer].sizeBef; nodeBef++)
			{
				file << layers[layer].weights[nodeAft][nodeBef];
				if (nodeBef == layers[layer].sizeBef - 1)
					file << ';';
				else
					file << ',';
			}
			file << '\n';
		}
	}

	file.close();
	return true;
}
bool Ai::save() const
{
	OPENFILENAMEA file;
	char path[100];

	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = NULL;
	file.lpstrFilter = (PSTR)"Text (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	file.lpstrFile = path;
	file.lpstrFile[0] = '\0';
	file.nMaxFile = sizeof(path);
	file.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	file.lpstrDefExt = (LPCSTR)L"txt";

	if (GetSaveFileNameA(&file) == false)
	{
		return false;
	}
	else
	{
		return save((std::string)path);
	}
}