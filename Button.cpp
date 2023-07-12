#include "Button.h"

Button::Button() {}

Button::Button(std::string text, int charSize, sf::Color contentColor, sf::Vector2f bPos, sf::Vector2f bSize, sf::Color bodyColor, const sf::Font& font) {
    sf::RectangleShape b(bSize);
    b.setFillColor(bodyColor);
    b.setPosition(bPos);
    body = b;
    originalBodyColor = bodyColor;
    originalContentColor = contentColor;

    hoverBodyColor = bodyColor;
    hoverContentColor = contentColor;

    activeBodyColor = bodyColor;
    activeContentColor = contentColor;
    content.setString(text);
    content.setFillColor(contentColor);
    content.setFont(font);
    content.setCharacterSize(charSize);

    content.setPosition(body.getPosition().x + (body.getSize().x) / 2 - (content.getGlobalBounds().width / 2), body.getPosition().y);
    coolDown = sf::seconds(0.1);
}

bool Button::contains(sf::Vector2f pos) {
    if (body.getPosition().x<pos.x && body.getPosition().x + body.getSize().x>pos.x &&
        body.getPosition().y<pos.y && body.getPosition().y + body.getSize().y>pos.y
        ) return true;
    else return false;
}

bool Button::click(sf::Vector2f pos) {
    if (contains(pos) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && timer <= sf::Time::Zero) {
        timer = coolDown;
        return true;

    }
    else return false;
}
void Button::update(sf::Vector2f pos,const sf::Time& dt) {
    if (click(pos)) {
        body.setFillColor(activeBodyColor);
        content.setFillColor(activeContentColor);
       // sprite.setColor(activeContentColor);
    }
    else if (contains(pos)) {
        body.setFillColor(hoverBodyColor);
        content.setFillColor(hoverContentColor);
        //sprite.setColor(hoverContentColor);
    }
    else {
        body.setFillColor(originalBodyColor);
        content.setFillColor(originalContentColor);
        //sprite.setColor(originalContentColor);
    }
    timer -= dt;
}

void Button::setSprite(std::string filePath, sf::Vector2f scale){
    if (!texture.loadFromFile(filePath)) {
        std::cout << "FAILED TO LOAD TEXTURE"<<std::endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(scale);
    sf::FloatRect s_rect = sprite.getGlobalBounds();
    sprite.setPosition(body.getPosition().x+(body.getSize().x - s_rect.width) / 2, body.getPosition().y + (body.getSize().y - s_rect.height)/2);
    sprite.setColor(originalContentColor);

}
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(body);
    target.draw(content);
    target.draw(sprite);
}