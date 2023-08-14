#include <SFML/Graphics.hpp>

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(), "neural network visualization", sf::Style::Default, settings);

	while (window.isOpen())
	{
		//handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				sf::View view(window.getView());
				view.zoom(1 - 0.1 * event.mouseWheelScroll.delta);
				window.setView(view);
			}
		}

		
	}

	return 0;
}