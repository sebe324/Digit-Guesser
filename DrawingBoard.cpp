#include "DrawingBoard.h"

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

	drawingBoardBounds = sf::FloatRect(position.x,position.y,position.x+cellEdgeSize*valuesWidth,cellEdgeSize*valuesHeight);
}

void DrawingBoard::updateCells()
{
	//set the color
	for (int i = 0; i < values.size(); i++)
		for (int j = 0; j < 4; j++) cells[i * 4 + j].color = cellColor*(values[i]/255);

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
void DrawingBoard::hover(sf::Vector2f mousePos)
{
	isHovered = drawingBoardBounds.contains(mousePos);
}

void DrawingBoard::click(sf::Vector2f mousePos)
{
	
	if (drawingBoardBounds.contains(mousePos)) {
		int row = Utils::clamp<int>(0, valuesHeight, (mousePos.y - position.y) / valuesHeight);
		int col = Utils::clamp<int>(0, valuesWidth, (mousePos.x - position.x) / valuesWidth);
		if (mode == WRITE) {
			//Add values

			
		}
		else {
			//remove values
		}
		updateCells();
	}
}

void DrawingBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(cells);
	target.draw(border);
}