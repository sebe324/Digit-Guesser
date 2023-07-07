#include <iostream>
#include <SFML/Graphics.hpp>
#include "DrawingBoard.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 800), "Number Guesser");
	window.setFramerateLimit(60);

	DrawingBoard drawingBoard(28, 28, 15, 200, 200);
	drawingBoard.values[0] = 255;
	drawingBoard.updateCells();
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
				break;
			}
		}
		window.clear();
		window.draw(drawingBoard);
		window.display();
	}
	return 0;
}