#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Utils.h"
//Drawing board that let's the user draw the digit.

enum Mode
{
WRITE,
ERASE
};

//utility for multiplying a color by a number.
sf::Color operator* (const sf::Color& left, float n);

class DrawingBoard : public sf::Drawable
{
	const unsigned valuesWidth;
	const unsigned valuesHeight;

	const float cellEdgeSize; //edge size for values to draw.

	
	sf::FloatRect drawingBoardBounds;

	unsigned cellsAmount;

	sf::Color cellColor = sf::Color::White;

	sf::RectangleShape border; //Border for the drawing board.

	sf::Vector2f position;

	sf::RectangleShape highlight;
	
	bool isHovered = false;
public:

	Mode mode=WRITE;
	unsigned char brushSize=1;
	std::vector<unsigned char> values; //Values in gray scale from 0 to 255.

	sf::VertexArray cells; //Vertex array used to represent the values vector on the screen.

	DrawingBoard(unsigned valuesWidth, unsigned valuesHeight, float cellEdgeSize, float positionX, float positionY);

	//If mouse hovers over the drawing board, the affected cells will be highlighted.
	void hover(sf::Vector2f mousePos); 

	//If user clicks then the highlighted cells will be coloured or cleared.
	void click(sf::Vector2f mousePos);

	void updateCells();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};