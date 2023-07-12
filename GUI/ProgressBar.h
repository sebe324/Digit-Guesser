#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iomanip>
#include <sstream>

class ProgressBar : public sf::Drawable{
	private:

	double maxValue;
	double currentValue;
	double targetValue;
	sf::Color backgroundColor;
	sf::Color barColor;

	double speed;

	sf::Text textValue;

	sf::Text textLabel;
public:

	sf::RectangleShape background;
	sf::RectangleShape bar;

	ProgressBar();
	ProgressBar(const sf::Vector2f& position, const sf::Vector2f& backgroundSize, const sf::Color& backgroundColor, const sf::Color& barColor, double maxValue, const sf::Font& font);

	void update(const sf::Time& deltaTime);

	void setValue(double v);

	void setMaxValue(double v);

	void setSpeed(double s);

	void setLabel(const std::string& l);

	double getCurrentValue();

	double getTargetValue();

	double getMaxValue();

	double getSpeed();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};