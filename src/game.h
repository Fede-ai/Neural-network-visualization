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
    void drawSliders();

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
    sf::RectangleShape slider;
	float sliderScroll = 0;
    int nnHeight = maxSize * 100 + (maxSize + 1) * 80;
	std::vector<double> inputs;
    int editingValue = -1;
    bool editedValue = false;
};