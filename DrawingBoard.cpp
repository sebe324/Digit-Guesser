#include "DrawingBoard.h"
#include <iostream>
sf::Color operator* (const sf::Color& left, float n)
{
	return sf::Color(left.r * n, left.g * n, left.b * n, left.a * n);
}
DrawingBoard::DrawingBoard(unsigned valuesWidth, unsigned valuesHeight, float cellEdgeSize, float positionX, float positionY) :
valuesWidth(valuesWidth), valuesHeight(valuesHeight), cellEdgeSize(cellEdgeSize), position(positionX,positionY)
{
	values.resize(valuesHeight * valuesWidth);
	
	
	cells.setPrimitiveType(sf::Quads);
	cellsAmount = values.size() * 4;
	cells.resize(cellsAmount);
	updateCells();

	border.setPosition(position);
	border.setFillColor(sf::Color(0, 0, 0, 0));
	border.setSize(sf::Vector2f(valuesWidth * cellEdgeSize, valuesHeight * cellEdgeSize));
	border.setOutlineColor(sf::Color(120,120,120));
	border.setOutlineThickness(5.f);

	highlight.setSize(sf::Vector2f(brushSize*cellEdgeSize,brushSize*cellEdgeSize));
	highlight.setOutlineColor(sf::Color::Yellow);
	highlight.setOutlineThickness(2);
	highlight.setFillColor(sf::Color::Transparent);
	drawingBoardBounds = sf::FloatRect(position.x,position.y,cellEdgeSize*valuesWidth,cellEdgeSize*valuesHeight);
}

void DrawingBoard::updateCells()
{
	//set the color
	for (int i = 0; i < values.size(); i++)
		for (int j = 0; j < 4; j++) cells[i * 4 + j].color = cellColor*(float(values[i])/255);

	//Set correct position of cells
	//This may be a little confusing. Each cell has 4 corners. The loop iterates through all corners and sets their positions.
	//0 - top left corner 
	//1 - top right corner
	//2 - bottom right corner
	//3 - bottom left corner

	for (unsigned row = 0; row < valuesHeight; row++)
	{
		for (unsigned col = 0; col < valuesWidth; col++)
		{
			cells[(row * valuesWidth + col)*4 + 0].position = sf::Vector2f(col*cellEdgeSize,row*cellEdgeSize)+position;
			cells[(row * valuesWidth + col)*4 + 1].position = sf::Vector2f(col*cellEdgeSize+cellEdgeSize,row*cellEdgeSize)+position;
			cells[(row * valuesWidth + col)*4 + 2].position = sf::Vector2f(col * cellEdgeSize + cellEdgeSize, row * cellEdgeSize+cellEdgeSize)+position;
			cells[(row * valuesWidth + col)*4 + 3].position = sf::Vector2f(col * cellEdgeSize, row * cellEdgeSize+cellEdgeSize)+position;
		}
	}
}
void DrawingBoard::hover(const sf::Vector2f& mousePos)
{
	isHovered = drawingBoardBounds.contains(mousePos);
	highlight.setPosition(mousePos);
}

void DrawingBoard::click(const sf::Vector2f& mousePos)
{
	
	if (drawingBoardBounds.contains(mousePos)) {
		int row = Utils::clamp(0, valuesHeight, (mousePos.y - position.y) / cellEdgeSize);
		int col = Utils::clamp(0, valuesWidth, (mousePos.x - position.x) / cellEdgeSize);
		switch (mode)
		{
			case WRITE:
				for (int i = 0; i < brushSize; i++) {
					for (int j = 0; j < brushSize; j++) {
						if(row+i<valuesHeight && col+j<valuesWidth)
						values[(row + i) * valuesWidth + col + j] = Utils::clamp(0, 255, values[(row + i) * valuesWidth + col + j] + 100);
					}
				}
			break;
			case ERASE:
				for (int i = 0; i < brushSize; i++) {
					for (int j = 0; j < brushSize; j++) {
						if (row + i < valuesHeight && col + j < valuesWidth)
							values[(row + i) * valuesWidth + col + j] = 0;
					}
				}
			break;
		}
		updateCells();
	}
}

void DrawingBoard::changeBrushSize(unsigned n)
{
	brushSize = Utils::clamp(1,6,n);
	highlight.setSize(sf::Vector2f(brushSize * cellEdgeSize, brushSize * cellEdgeSize));
}

void DrawingBoard::changeMode(Mode m)
{
	mode = m;
	switch (mode)
	{
	case WRITE:
		highlight.setOutlineColor(sf::Color::Yellow);
	break;
	case ERASE:
		highlight.setOutlineColor(sf::Color::Red);
	break;
	}
}

void DrawingBoard::clear()
{
	for (unsigned& x : values) x = 0;
	updateCells();
}
void DrawingBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(cells);
	target.draw(border);
	if (isHovered)
		target.draw(highlight);
}

std::vector<double> DrawingBoard::getValuesFrom0To1()
{
	std::vector<double> result(values.size());
	for (int i = 0; i < values.size(); i++) {
		result[i] = values[i] / 255.0;
	}
	return result;
}