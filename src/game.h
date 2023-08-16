#pragma once
#include <SFML/Graphics.hpp>
#include "ai.h"

class Game
{
public:
    void play();

private:    
    void update();
    void draw();

    void setView();
    void initAi();

    Ai* ai = nullptr;
    bool pressed = false;
    sf::RenderWindow window;
	sf::RenderTexture nn;
    sf::Vector2i lastMousePos;
	bool isMooving = false;
	sf::Font font;
	sf::Text text;
	int maxSize = 0;
	std::vector<int> aiSize;
	sf::CircleShape neuron;	
	sf::VertexArray weight;
	sf::RectangleShape sliderBg;
	float sliderScroll = 0;
	std::vector<double> lastInputs;
};