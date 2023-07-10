#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Button : public sf::Drawable {
public:

    sf::RectangleShape body;
    sf::Text content;
    sf::Color originalBodyColor;
    sf::Color originalContentColor;

    sf::Color hoverBodyColor;
    sf::Color hoverContentColor;

    sf::Color activeBodyColor;
    sf::Color activeContentColor;

    sf::Time coolDown;
    sf::Time timer=sf::Time::Zero;
    bool contains(sf::Vector2f pos);
    bool click(sf::Vector2f pos);
    void update(sf::Vector2f pos, const sf::Time& dt);
    Button();
    Button(std::string text, int charSize, sf::Color contentColor, sf::Vector2f bPos, sf::Vector2f bSize, sf::Color bodyColor, sf::Font& font);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};