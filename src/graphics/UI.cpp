#include "UI.h"
#include "../core/Config.h"
#include <string>

UI::UI()
    : tempSlider(30.0f, 40.0f, 300.0f),
    font("C:/Windows/Fonts/arial.ttf"),
    tempLabel(font, "", 16)
{
    tempLabel.setFillColor(sf::Color::White);
    tempLabel.setPosition(sf::Vector2f(30.0f, 10.0f));
}

void UI::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    tempSlider.handleEvent(event, window);
}

void UI::draw(sf::RenderWindow& window) {
    tempSlider.draw(window);
    tempLabel.setString("Temperature: " + std::to_string(static_cast<int>(getTargetTemperature())));
    window.draw(tempLabel);
}

float UI::getTargetTemperature() const {
    return tempSlider.getValue() * Config::MAX_TEMPERATURE;
}