#include <iostream>
#include <SFML/Graphics.hpp>
#include "DrawingBoard.h"
#include "Button.h"
#include <string>

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
	title.setPosition(sf::Vector2f((windowWidth-title.getGlobalBounds().width)/2,50));
	title.setCharacterSize(40);

	sf::Text brushSizeLabel;
	brushSizeLabel.setFont(font);
	brushSizeLabel.setFillColor(sf::Color::White);
	brushSizeLabel.setString("Brush Size: ");
	brushSizeLabel.setPosition(sf::Vector2f(200,650));

	sf::Text brushSizeValue;
	brushSizeValue.setFont(font);
	brushSizeValue.setFillColor(sf::Color::White);
	brushSizeValue.setString(std::to_string(drawingBoard.brushSize));
	brushSizeValue.setPosition(sf::Vector2f(524,650));
	Button buttonBrushSizeInc("+", 40, sf::Color::White, sf::Vector2f(434, 650), sf::Vector2f(50, 50), sf::Color::Black, font);
	Button buttonBrushSizeDec("-", 40, sf::Color::White, sf::Vector2f(574, 650), sf::Vector2f(50, 50), sf::Color::Black, font);
	buttonBrushSizeInc.hoverBodyColor = sf::Color(50, 50, 50);
	buttonBrushSizeDec.hoverBodyColor = sf::Color(50, 50, 50);
	buttonBrushSizeInc.body.setOutlineThickness(2);
	buttonBrushSizeInc.body.setOutlineColor(sf::Color::White);
	buttonBrushSizeDec.body.setOutlineThickness(2);
	buttonBrushSizeDec.body.setOutlineColor(sf::Color::White);

	Button buttonStartPerceptron("START", 40, sf::Color::White, sf::Vector2f(0, 0), sf::Vector2f(200, 50), sf::Color::Black, font);
	buttonStartPerceptron.hoverBodyColor = sf::Color(50, 50, 50);

	Button buttonClearBoard("Clear", 40, sf::Color::White, sf::Vector2f(0, 0), sf::Vector2f(200, 50), sf::Color::Black, font);
	buttonClearBoard.hoverBodyColor = sf::Color(50, 50, 50);
	sf::Clock clock;
	sf::Time deltaTime = sf::seconds(0.016);
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
				break;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				drawingBoard.click(mousePos);
				if (buttonBrushSizeDec.click(mousePos)) {
					drawingBoard.changeBrushSize(drawingBoard.brushSize - 1);
					brushSizeValue.setString(std::to_string(drawingBoard.brushSize));
				}
				else if(buttonBrushSizeInc.click(mousePos)) {
					drawingBoard.changeBrushSize(drawingBoard.brushSize + 1);
					brushSizeValue.setString(std::to_string(drawingBoard.brushSize));
				}
			}
		}

		buttonBrushSizeDec.update(mousePos, deltaTime);
		buttonBrushSizeInc.update(mousePos, deltaTime);

		window.clear();
		window.draw(drawingBoard);
		window.draw(buttonBrushSizeInc);
		window.draw(buttonBrushSizeDec);
		window.draw(buttonStartPerceptron);
		window.draw(title);
		window.draw(brushSizeLabel);
		window.draw(brushSizeValue);
		window.display();
		deltaTime = clock.restart();
	}
	return 0;
}