#include "Slider.h"
#include <algorithm>

Slider::Slider(float x, float y, float width)
    : x(x), y(y), width(width)
{
    track.setSize(sf::Vector2f(width, 6.0f));
    track.setPosition(sf::Vector2f(x, y));
    track.setFillColor(sf::Color(60, 60, 60));

    knob.setRadius(10.0f);
    knob.setOrigin(sf::Vector2f(10.0f, 10.0f));
    knob.setFillColor(sf::Color(220, 220, 220));
    knob.setPosition(sf::Vector2f(x + width * value, y + 3.0f));
}

void Slider::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (const auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mouse(static_cast<float>(mb->position.x), static_cast<float>(mb->position.y));
        sf::FloatRect hitbox(sf::Vector2f(x - 15.0f, y - 15.0f), sf::Vector2f(width + 30.0f, 36.0f));
        if (hitbox.contains(mouse)) {
            dragging = true;
            value = std::clamp((mouse.x - x) / width, 0.0f, 1.0f);
            knob.setPosition(sf::Vector2f(x + width * value, y + 3.0f));
        }
    }
    if (event.is<sf::Event::MouseButtonReleased>()) {
        dragging = false;
    }
    if (const auto* mm = event.getIf<sf::Event::MouseMoved>()) {
        if (dragging) {
            sf::Vector2f mouse(static_cast<float>(mm->position.x), static_cast<float>(mm->position.y));
            value = std::clamp((mouse.x - x) / width, 0.0f, 1.0f);
            knob.setPosition(sf::Vector2f(x + width * value, y + 3.0f));
        }
    }
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(knob);
}