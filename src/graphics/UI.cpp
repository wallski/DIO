#include "UI.h"
#include "../core/Config.h"
#include <algorithm>
#include <cmath>
#include <string>

UI::UI()
    : font("C:/Windows/Fonts/arial.ttf"),
    tempSlider(30.0f, 40.0f, 300.0f),
    tempBox(font, 360.0f, 32.0f, 110.0f, 28.0f),
    tempLabel(font, "", 16),
    temperature(Config::DEFAULT_TEMPERATURE)
{
    tempLabel.setFillColor(sf::Color::White);
    tempLabel.setPosition(sf::Vector2f(30.0f, 10.0f));

    tempBox.setValue(temperature);

    tempBox.setOnCommit([this](float v) {
        v = std::clamp(v, Config::MIN_TEMPERATURE, Config::MAX_TEMPERATURE);
        temperature = v;
        float normalized = (v - Config::MIN_TEMPERATURE) /
            (Config::MAX_TEMPERATURE - Config::MIN_TEMPERATURE);
        tempSlider.setNormalizedValue(normalized);
        });
}

void UI::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    tempSlider.handleEvent(event, window);
    tempBox.handleEvent(event);
}

void UI::draw(sf::RenderWindow& window) {
    float sliderTemp = Config::MIN_TEMPERATURE +
        tempSlider.getValue() * (Config::MAX_TEMPERATURE - Config::MIN_TEMPERATURE);

    if (std::abs(sliderTemp - temperature) > 1.0f && !tempBox.isFocused()) {
        temperature = sliderTemp;
    }

    if (!tempBox.isFocused()) {
        tempBox.setValue(temperature);
    }

    tempSlider.draw(window);
    tempBox.draw(window);

    float kelvin = temperature;
    float celsius = kelvin - 273.15f;
    std::string label = "Temperature: " + std::to_string(static_cast<int>(kelvin)) +
        " K (" + std::to_string(static_cast<int>(celsius)) + " C)";
    tempLabel.setString(label);
    window.draw(tempLabel);
}