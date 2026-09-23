#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class TextBox {
public:
    TextBox(const sf::Font& font, float x, float y, float width, float height);
    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);

    void setValue(float v);
    float getValue() const { return value; }
    bool isFocused() const { return focused; }
    void setOnCommit(std::function<void(float)> cb) { onCommit = cb; }

private:
    void commit();
    void updateText();

    float value = 0.0f;
    std::string buffer;
    bool focused = false;

    sf::RectangleShape box;
    sf::Text text;
    std::function<void(float)> onCommit;
};