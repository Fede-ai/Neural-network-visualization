#pragma once
#include <SFML/Graphics.hpp>
#include "ai.h"

class Bp
{
public:
    void run();

private:    
    void update();
    void draw();
    void computeImage();
    void initAi();

    Ai* ai = nullptr;
    int frame = 1;
    bool pressed = false;
    bool canCenter = false;
    bool isFullscreen = false;
    bool canFullscreen = false;
    bool canSave = false;
    bool canLoad = false;
    sf::RenderWindow window;

    sf::Texture textureImage;
    sf::Image imageImage;
    sf::Sprite image;
    sf::CircleShape origin;

    bool isLearning = false;
    bool canLearn = false;
    sf::Vector2i lastMousePos;
    std::vector<DataPoint> datapoints;
	bool isMooving = false;
    sf::Vector2f imageSize = sf::Vector2f(400, 225);
};