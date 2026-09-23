#include "TextBox.h"
#include <cstdlib>

TextBox::TextBox(const sf::Font& font, float x, float y, float width, float height)
    : text(font, "", 16)
{
    box.setSize(sf::Vector2f(width, height));
    box.setPosition(sf::Vector2f(x, y));
    box.setFillColor(sf::Color(30, 30, 30));
    box.setOutlineColor(sf::Color(80, 80, 80));
    box.setOutlineThickness(1.0f);

    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(x + 8.0f, y + 5.0f));
    text.setString("0");
}

void TextBox::setValue(float v) {
    value = v;
    if (!focused) updateText();
}

void TextBox::updateText() {
    if (focused) {
        text.setString(buffer.empty() ? "_" : buffer + "_");
    }
    else {
        text.setString(std::to_string(static_cast<int>(value)));
    }
}

void TextBox::commit() {
    if (!buffer.empty()) {
        float v = static_cast<float>(std::atof(buffer.c_str()));
        value = v;
        if (onCommit) onCommit(v);
    }
    buffer.clear();
    focused = false;
    updateText();
}

void TextBox::handleEvent(const sf::Event& event) {
    if (const auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            sf::Vector2f mouse(static_cast<float>(mb->position.x), static_cast<float>(mb->position.y));
            sf::FloatRect bounds(box.getPosition(), box.getSize());
            if (bounds.contains(mouse)) {
                focused = true;
                buffer.clear();
                updateText();
            }
            else if (focused) {
                commit();
            }
        }
    }

    if (!focused) return;

    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        if (kp->code == sf::Keyboard::Key::Backspace) {
            if (!buffer.empty()) buffer.pop_back();
            updateText();
        }
        if (kp->code == sf::Keyboard::Key::Enter) {
            commit();
        }
    }

    if (const auto* te = event.getIf<sf::Event::TextEntered>()) {
        char32_t u = te->unicode;
        if (u >= U'0' && u <= U'9') {
            if (buffer.size() < 8) buffer += static_cast<char>(u);
            updateText();
        }
        else if (u == U'.') {
            if (buffer.find('.') == std::string::npos) buffer += '.';
            updateText();
        }
    }
}

void TextBox::draw(sf::RenderWindow& window) {
    box.setOutlineColor(focused ? sf::Color(220, 220, 220) : sf::Color(80, 80, 80));
    window.draw(box);
    window.draw(text);
}