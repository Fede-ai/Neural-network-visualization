#pragma once
#include <SFML/Graphics.hpp>
#include "ai.h"

class Game
{
public:
    void play();

private:    
    void update();
    void drawNn();
    void drawImage();
    void drawSliders();

    void computeImage();    
    void initAi();
    void setView();

    Ai* ai = nullptr;
    bool pressed = false;
    bool isNnFocused = true;
    bool drawViewport = true;
    bool isFullscreen = false;
    bool canFullscreen = false;
    sf::RenderWindow window;

	sf::RenderTexture nnSpace;
    sf::RectangleShape nnCanvas;	
    sf::RenderTexture imageSpace;
    sf::RectangleShape imageCanvas;	

    sf::Texture textureImage;
    sf::Image imageImage;
    sf::Sprite image;

    sf::Vector2i lastMousePos;
    sf::CircleShape origin;
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
    int nnHeight;
	std::vector<double> inputs;
    int editingValue = -1;
};