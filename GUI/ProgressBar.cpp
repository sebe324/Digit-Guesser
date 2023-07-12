#include "ProgressBar.h"

ProgressBar::ProgressBar(){}


ProgressBar::ProgressBar(const sf::Vector2f& position, const sf::Vector2f& backgroundSize, const sf::Color& backgroundColor, const sf::Color& barColor, double maxValue, const sf::Font& font)
{
	background.setSize(backgroundSize);
	background.setFillColor(backgroundColor);
	background.setPosition(position);
	
	bar.setSize(sf::Vector2f(0.f, backgroundSize.y));
	bar.setFillColor(barColor);
	bar.setPosition(position);

	this->maxValue = maxValue;
	this->currentValue = 0.0;
	this->targetValue = 0.0;
	speed = 5;

	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << currentValue;
	textValue = sf::Text(stream.str(), font, backgroundSize.y * 0.6);
	textValue.setPosition(sf::Vector2f(position.x+backgroundSize.x+10.f,position.y));
	textValue.setFillColor(barColor);
	textLabel = sf::Text("", font, backgroundSize.y * 0.6);
	textLabel.setPosition(sf::Vector2f(position.x - 25.f, position.y));
}

void ProgressBar::update(const sf::Time& deltaTime)
{
	if (targetValue - currentValue > speed * deltaTime.asSeconds()) {
		currentValue += speed * deltaTime.asSeconds();
		bar.setSize(sf::Vector2f(currentValue / maxValue * background.getSize().x, background.getSize().y));
		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << currentValue;
		textValue.setString(stream.str());
	}
	else if (targetValue - currentValue < speed * deltaTime.asSeconds() && targetValue - currentValue>0.0) {
		currentValue = targetValue;
		bar.setSize(sf::Vector2f(currentValue / maxValue * background.getSize().x, background.getSize().y));
		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << currentValue;
		textValue.setString(stream.str());
	}
	
}

void ProgressBar::setValue(double v)
{
	currentValue = 0.0;
	bar.setSize(sf::Vector2f(0.f, background.getSize().y));
	if (v < 0) targetValue = 0;
	else if (v > maxValue) targetValue = maxValue;
	else targetValue = v;
}

void ProgressBar::setMaxValue(double v)
{
	if (v < 0) maxValue = 0.01;
	else maxValue = v;
}
void ProgressBar::setSpeed(double s) {
	if (speed < 0) speed = 0;
	else speed = s; 
}

void ProgressBar::setLabel(const std::string& l) {
	textLabel.setString(l);
	textLabel.setPosition(sf::Vector2f(background.getPosition().x - l.length()*textLabel.getCharacterSize(), background.getPosition().y));
}
double ProgressBar::getCurrentValue()
{
	return currentValue;
}

double ProgressBar::getTargetValue()
{
	return targetValue;
}

double ProgressBar::getMaxValue()
{
	return maxValue;
}

double ProgressBar::getSpeed()
{
	return speed;
}
void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	target.draw(background);
	target.draw(bar);
	target.draw(textValue);
	target.draw(textLabel);
}