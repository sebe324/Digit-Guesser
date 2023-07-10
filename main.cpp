#include <iostream>
#include <SFML/Graphics.hpp>
#include "DrawingBoard.h"

int main()
{
	const int windowWidth=1200;
	const int windowHeight=800;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Number Guesser");
	window.setFramerateLimit(60);

	DrawingBoard drawingBoard(28, 28, 15,200, 200);
	drawingBoard.updateCells();
	sf::Vector2f mousePos;

	sf::Font font;
	if (!font.loadFromFile("font.ttf")) return -1;

	sf::Text title;
	title.setFont(font);
	title.setFillColor(sf::Color::White);
	title.setString("Digit Guesser");
	title.setPosition(sf::Vector2f((windowWidth-title.getGlobalBounds().width)/2,100));
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
				case sf::Event::MouseMoved:
					mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					drawingBoard.hover(mousePos);
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) drawingBoard.click(mousePos);
				break;
			}
		}
		window.clear();
		window.draw(drawingBoard);
		window.draw(title);
		window.display();
	}
	return 0;
}