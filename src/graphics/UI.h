#pragma once
#include <SFML/Graphics.hpp>
#include "Slider.h"
#include "TextBox.h"

class UI {
public:
    UI();
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    float getTargetTemperature() const { return temperature; }

private:
    sf::Font font;
    Slider tempSlider;
    TextBox tempBox;
    sf::Text tempLabel;
    float temperature;
};