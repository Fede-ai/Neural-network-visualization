#include "game.h"
#include <iostream>

void Game::play()
{
    initAi();
    ai->setRandomValues();

    for (int i = 0; i < aiSize.size(); i++)
	{
		maxSize = std::max(maxSize, aiSize[i]);
	}
	
	neuron.setRadius(50);
	neuron.setOrigin(50, 50);
	neuron.setOutlineThickness(5);
	neuron.setOutlineColor(sf::Color::Black);

	lastInputs.clear();
	lastInputs.push_back(100);
	lastInputs.push_back(100);
	ai->calculateOutput(lastInputs);

	weight.setPrimitiveType(sf::Lines);
	weight.resize(2);

	font.loadFromFile("./resources/font.ttf");
	text.setFillColor(sf::Color::Black);
	text.setFont(font);
	text.setCharacterSize(60);
	text.setScale(0.25, 0.25);
	lastMousePos = sf::Mouse::getPosition();

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	nn.setSmooth(true);
	nn.create(1600, 1080, settings);
	setView();

	window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width * 2/3, sf::VideoMode::getDesktopMode().width * 3/8), "neural network visualization", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    window.setView(sf::View(sf::Vector2f(1920/2, 1080/2), sf::Vector2f(1920, 1080)));
    window.requestFocus();

    bool isFullscreen = false;
    bool canFullscreen = false;

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
                if (window.mapPixelToCoords(sf::Mouse::getPosition(window)).x > 320)
		        {
		    	    sf::View view(nn.getView());
		    	    if (scroll > 0)
		    	    	view.zoom(1 - 0.1 * scroll);
		    	    else
		    		view.zoom(1 / (1 + 0.1*scroll));
		    	    nn.setView(view);			
		        }
		        else
		        {
			        sliderScroll = std::max(sliderScroll + scroll * 24, 0.f);
		        }
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
							window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width * 2/3, sf::VideoMode::getDesktopMode().width * 3/8), "simple", sf::Style::Default, settings);
							window.setFramerateLimit(60);
							window.setView(view);
						}
						else
						{	
                            sf::View view(window.getView());
							window.create(sf::VideoMode(), "simple", sf::Style::Fullscreen, settings);
							window.setFramerateLimit(60);
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

		window.clear(sf::Color(150, 150, 150));

        update();
        draw();

        window.display();
	}
}

void Game::update()
{
	if (pressed && window.mapPixelToCoords(sf::Mouse::getPosition(window)).x > 320)
		isMooving = true;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isMooving)
    {
        sf::View newView(nn.getView());
		sf::Vector2f diff = sf::Vector2f(lastMousePos - sf::Mouse::getPosition());
		diff = sf::Vector2f(diff.x *nn.getView().getSize().x/window.getSize().x*1.2, diff.y *nn.getView().getSize().y/window.getSize().y);
        newView.move(diff);
        nn.setView(newView);
    }
	else
	{
		isMooving = false;
	}

	lastMousePos = sf::Mouse::getPosition();
}

void Game::draw()
{
    nn.clear(sf::Color(200, 200, 200));
	int height = maxSize * 100 + (maxSize + 1) * 80;
	
	//draw weights
	for (int layer = 1; layer < aiSize.size(); layer++)
	{
		float space = (height - 100 * aiSize[layer]) / (aiSize[layer] + 1);
		float spaceBefore = (height - 100 * aiSize[layer - 1]) / (aiSize[layer - 1] + 1);
		for (int neur = 0; neur < aiSize[layer]; neur++)
		{
			weight[1].position = sf::Vector2f(120 + 300 * layer, (space + 100) * neur + space + 50);
			for (int neurBefore = 0; neurBefore < aiSize[layer - 1]; neurBefore++)
			{
				weight[0].position = sf::Vector2f(120 + 300 * (layer - 1), (spaceBefore + 100) * neurBefore + spaceBefore + 50);
				nn.draw(weight);
			}
		}
	}

	//draw neuron
	for (int layer = 0; layer < aiSize.size(); layer++)
	{
		float space = (height - 100 * aiSize[layer]) / (aiSize[layer] + 1);
		for (int neur = 0; neur < aiSize[layer]; neur++)
		{
			if (layer > 0){}
				//neuron.setFillColor(sf::Color((1 - ai->getNeuron(layer - 1, neur)) * 255, ai->getNeuron(layer - 1, neur) * 255, 0));
			else
				neuron.setFillColor(sf::Color::White);
			neuron.setPosition(120 + 300 * layer, (space + 100) * neur + space + 50);
			nn.draw(neuron);
		}
	}

	//draw text
	for (int layer = 1; layer < aiSize.size(); layer++)
	{
		float space = (height - 100 * aiSize[layer]) / (aiSize[layer] + 1);
		float spaceBefore = (height - 100 * aiSize[layer - 1]) / (aiSize[layer - 1] + 1);
		for (int neur = 0; neur < aiSize[layer]; neur++)
		{
			for (int neurBefore = 0; neurBefore < aiSize[layer - 1]; neurBefore++)
			{
				//draw weight text
				float xDiff = 300;
				float yDiff = (space + 100) * neur + space + 50 - ((spaceBefore + 100) * neurBefore + spaceBefore + 50);
				std::string weight = std::to_string((int)round(ai->getWeight(layer - 1, neur, neurBefore)*1000)); 
				if (weight.at(0) == '-')
				{
					if (weight.size() == 5)
					{
						weight.insert(2, ".");
					}
					else
					{
						weight.insert(1, "0.");
					}
				}
				else
				{
					if (weight.size() == 4)
					{
						weight.insert(1, ".");
					}
					else
					{
						weight.insert(0, "0.");
					}
				}
				text.setString(weight);
				text.setOrigin(text.getGlobalBounds().width*2, text.getGlobalBounds().height*2);
				text.setRotation(atan(yDiff/xDiff) * 180 / 3.1415);
				text.setPosition(120 + 300 * layer - 150, ((space + 100) * neur + space + 50 + (spaceBefore + 100) * neurBefore + spaceBefore + 50) /2 - 5);
				nn.draw(text);
				text.setRotation(0);
			}
			//draw neuron text
			text.setString(std::to_string(ai->getNeuron(layer - 1, neur)));
			text.setOrigin(text.getGlobalBounds().width*2, text.getGlobalBounds().height*2);
			text.setPosition(120 + 300 * layer, (space + 100) * neur + space + 50 - 5);
			nn.draw(text);

			//draw bias text
			std::string bias = std::to_string((int)round(ai->getBias(layer - 1, neur)*1000));
			if (bias.at(0) == '-')
			{
				if (bias.size() == 5)
				{
					bias.insert(2, ".");
				}
				else
				{
					bias.insert(1, "0.");
				}
			}
			else
			{
				if (bias.size() == 4)
				{
					bias.insert(1, ".");
				}
				else
				{
					bias.insert(0, "0.");
				}
			}
			text.setString("bias: " + bias);
			text.setOrigin(text.getGlobalBounds().width*2, text.getGlobalBounds().height*2);
			text.setPosition(120 + 300 * layer, (space + 100) * neur + space - 15);
			nn.draw(text);
		}
	}

	//draw input layer neuron text
	for (int input = 0; input < aiSize[0]; input++)
	{
		float space = (height - 100 * aiSize[0]) / (aiSize[0] + 1);
		text.setString(std::to_string(lastInputs[input]));
		text.setOrigin(text.getGlobalBounds().width*2, text.getGlobalBounds().height*2);
		text.setPosition(120, (space + 100) * input + space + 50 - 5);
		nn.draw(text);
	}
	
	nn.display();
	sf::RectangleShape nnImage;
	nnImage.setSize(sf::Vector2f(1600, 1080));
	nnImage.setPosition(sf::Vector2f(320, 0));
	nnImage.setTexture(&nn.getTexture());

	window.draw(nnImage);
}

void Game::initAi()
{
    aiSize.clear();
    aiSize.push_back(2);
    std::string cmd;
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
        if (!cmd.empty() && it == cmd.end())
        {
            aiSize.push_back(stoi(cmd));
        }  

        std::cout << "\n";
        //system("CLS");
    } while (cmd != "start");

    
    do {
        std::cout << "current network's size: ";
        for (int i = 0; i < aiSize.size(); i++)
        {
            std::cout << aiSize[i] << ", ";
        }
        std::cout << "_.\n"; 

        std::cout << "do you want to create an RBG image (\'rgb\') or a black and white one (\'bw\')? ";
        std::cin >> cmd;

        if (cmd != "rgb" && cmd != "bw")
        {
            std::cout << "\n";
            //system("CLS");
        }
    } while (cmd != "rgb" && cmd != "bw");

    std::cout << "final network's size: ";
    for (int i = 0; i < aiSize.size(); i++)
    {
        std::cout << aiSize[i] << ", ";
    }

    if (cmd == "rgb")
    {
        aiSize.push_back(3);
        std::cout << "3.\n";        
    }
    else
    {
        aiSize.push_back(2);
        std::cout << "2.\n";        
    }

    ai = new Ai(aiSize);
}

void Game::setView()
{
	float height = maxSize * 100 + (maxSize + 1) * 80;
	float width = 300 * (aiSize.size() - 1) + 240;
	float prop = 1600 / 1080.f;

	if (height * prop > width)
	{
		nn.setView(sf::View(sf::Vector2f(width/2, height/2), sf::Vector2f(height * prop, height)));
	}
	else
	{
		nn.setView(sf::View(sf::Vector2f(width/2, height/2), sf::Vector2f(width, width / prop)));
	}
}