#include "bp.h"
#include <iostream>
#include <fstream>
#include <thread>

void Bp::run()
{	
	initAi();
	imageImage.create(imageSize.x, imageSize.y, sf::Color(200, 200, 200));

	origin.setRadius(3);
	origin.setOrigin(3, 3);
	origin.setFillColor(sf::Color::Green);
	lastMousePos = sf::Mouse::getPosition();
	
	window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width * 2/3, sf::VideoMode::getDesktopMode().width * 3/8), "BackPropagation");
    window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(imageImage.getSize().x, imageImage.getSize().y)));
    window.requestFocus();	
	
	computeImage();

	while (window.isOpen())
	{
        pressed = false;
		//handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
            switch (event.type)
            {
			case sf::Event::Closed:
				window.close();
                break;
			case sf::Event::MouseWheelScrolled:
            {
                float scroll = event.mouseWheelScroll.delta;
				sf::View view(window.getView());
				if (scroll > 0)
					view.zoom(1 - 0.1 * scroll);
				else
					view.zoom(1 / (1 + 0.1*scroll));
				window.setView(view);		
                break;                
            }
            case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{
					if (canFullscreen)
					{
						if (isFullscreen)
						{
                            sf::View view(window.getView());
							window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width * 2/3, sf::VideoMode::getDesktopMode().width * 3/8), "Backpropagation", sf::Style::Default);
							window.setView(view);
						}
						else
						{	
                            sf::View view(window.getView());
							window.create(sf::VideoMode(), "simple", sf::Style::Fullscreen);
							window.setView(view);
						}
						isFullscreen = !isFullscreen;
					}
					canFullscreen = false;
				}
				else
				{
					canFullscreen = true;
				}
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                    pressed = true;
                break;
            }
		}
	
		update();
		draw();
	}
}

void Bp::update()
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (pressed)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			DataPoint newDatapoint;
			newDatapoint.targets.push_back(0);
			newDatapoint.targets.push_back(1);
			newDatapoint.inputs.push_back(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/(imageSize.x/2.f));
			newDatapoint.inputs.push_back(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/(imageSize.y/2.f));
			datapoints.push_back(newDatapoint);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		{
			DataPoint newDatapoint;
			newDatapoint.targets.push_back(1);
			newDatapoint.targets.push_back(0);
			newDatapoint.inputs.push_back(window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/(imageSize.x/2.f));
			newDatapoint.inputs.push_back(window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/(imageSize.y/2.f));
			datapoints.push_back(newDatapoint);
		}
		else
		{
			isMooving = true;	
		}
	}

	if (datapoints.size() > 0)
	{	
		if (isLearning)
		{
			ai->learn(datapoints, 0.1);
			std::cout << ai->cost(datapoints) << ", " << frame << "\n";
			frame++;			
		}
	}			
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		computeImage();	

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isMooving)
    {
		sf::View view(window.getView());
		sf::Vector2f diff = sf::Vector2f(lastMousePos - sf::Mouse::getPosition());
		diff = sf::Vector2f(diff.x *window.getView().getSize().x/window.getSize().x, diff.y *window.getView().getSize().y/window.getSize().y);
        view.move(diff);
		window.setView(view);
		lastMousePos = sf::Mouse::getPosition();
    }
	else
	{
		lastMousePos = sf::Mouse::getPosition();
		isMooving = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		if (canCenter)
		{
			window.setView(sf::View(sf::Vector2f(0, 0), sf::Vector2f(imageImage.getSize().x, imageImage.getSize().y)));
			canCenter = false;
		}
	}
	else
	{
		canCenter = true;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		if (canLearn)
		{
			isLearning = !isLearning;
			canLearn = false;
		}
	}
	else
	{
		canLearn = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (canSave)
		{
			if (!ai->save())
				std::cout << "failed saving\n";
			canSave = false;
		}
	}
	else
	{
		canSave = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if (canLoad)
		{
			bool failed;
			Ai loadedAi(failed);
			if (failed)
			{
				std::cout << "failed loading\n";
			}
			else
			{
				*ai = loadedAi;
				datapoints.clear();
				computeImage();
			}

			canLoad = false;
		}
	}
	else
	{
		canLoad = true;
	}
}
void Bp::draw()
{
	textureImage.loadFromImage(imageImage);
	image.setTexture(textureImage);
	image.setPosition(window.getView().getCenter().x - window.getView().getSize().x/2, window.getView().getCenter().y - window.getView().getSize().y/2);
	image.setScale(window.getView().getSize().x / imageSize.x, window.getView().getSize().x / imageSize.x);
	window.draw(image);

	sf::CircleShape datapoint(3);
	datapoint.setOrigin(3, 3);
	datapoint.setScale(std::min(window.getView().getSize().x / imageSize.x, 3.f), std::min(window.getView().getSize().x / imageSize.x, 3.f));
	for (auto data : datapoints)
	{
		datapoint.setPosition(data.inputs[0] * (imageSize.x/2.f), data.inputs[1] * (imageSize.y/2.f));
		if (data.targets[0] == 0)
			datapoint.setFillColor(sf::Color::Yellow);
		else
			datapoint.setFillColor(sf::Color::Blue);
		window.draw(datapoint);
	}

	//datapoint used to indicate the 4 corners
	datapoint.setFillColor(sf::Color::Red);
	datapoint.setPosition(-imageSize.x/2, -imageSize.y/2);
	window.draw(datapoint);
	datapoint.setPosition(-imageSize.x/2, imageSize.y/2);
	window.draw(datapoint);
	datapoint.setPosition(imageSize.x/2, -imageSize.y/2);
	window.draw(datapoint);
	datapoint.setPosition(imageSize.x/2, imageSize.y/2);
	window.draw(datapoint);
	
	origin.setScale(std::min(window.getView().getSize().x / imageSize.x, 3.f), std::min(window.getView().getSize().x / imageSize.x, 3.f));
	window.draw(origin);

	window.display();
}

void Bp::computeImage()
{
	for (int y = 0; y < imageSize.y; y++)
	{
		for (int x = 0; x < imageSize.x; x++)
		{
			std::vector<double> imageInputs;
			sf::Vector2f inputsPos = window.mapPixelToCoords(sf::Vector2i(x*window.getSize().x/imageSize.x, y*window.getSize().y/imageSize.y));
			imageInputs.push_back(inputsPos.x/(imageSize.x/2.f));
			imageInputs.push_back(inputsPos.y/(imageSize.y/2.f));
			std::vector<double> output = ai->calculateOutput(imageInputs);

			if (output[0] > output[1])
				imageImage.setPixel(x, y, sf::Color::Black);
			else if (output[0] < output[1])
				imageImage.setPixel(x, y, sf::Color::White);	
			else
				imageImage.setPixel(x, y, sf::Color(150, 150, 150));	
		}
	}
}

void Bp::initAi()
{
	init:

	std::string cmd;
	std::cout << "do you want to load an existing network [ld] or create a new one [new]? ";
	std::cin >> cmd;
	while (cmd != "new" && cmd != "ld")
	{
		std::cout << "unrecognised command, try again: ";
		std::cin >> cmd;
	}
	std::cout << "\n";

	if (cmd == "new")
	{	
		ai = new Ai(2, 2);
		ai->setRandomValues();
	}
	else
	{
		bool failed;
		ai = new Ai(failed);

		while (failed)
		{
			delete ai;
			std::cout << "failed loading\n";
			goto init;
		}
	}
	std::cout << "\n";
}