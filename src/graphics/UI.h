#pragma once
#include <SFML/Graphics.hpp>
#include "Slider.h"

class UI {
public:
    UI();
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    float getTargetTemperature() const;

private:
    Slider tempSlider;
    sf::Font font;
    sf::Text tempLabel;
};