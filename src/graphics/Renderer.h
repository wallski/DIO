#pragma once
#include <SFML/Graphics.hpp>
#include "../physics/World.h"

class Renderer {
public:
    Renderer(sf::RenderWindow& window);
    void draw(const World& world);

private:
    sf::RenderWindow& window;
};