#pragma once
#include <SFML/Graphics.hpp>

class Slider {
public:
    Slider(float x, float y, float width);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    float getValue() const { return value; }

private:
    float x, y, width;
    float value = 0.5f;
    bool dragging = false;
    sf::RectangleShape track;
    sf::CircleShape knob;
};