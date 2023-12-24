#include "nn.h"
#include "bp.h"
#include <iostream>

int main()
{
	std::cout << "do you want to start a neural network visualization [nn]\n";
	std::cout << "or a backpropagation visualization [bp]? ";
	std::string cmd;
	std::cin >> cmd;

	while (cmd != "nn" && cmd != "bp")
	{
		std::cout << "invalid command, try again: ";
		std::cin >> cmd;
	}
	std::cout << "\n";

	if (cmd == "nn")
	{
		Nn nn;
		nn.run();
	}
	else
	{
		Bp bp;
		bp.run();
	}

	return 0;
}