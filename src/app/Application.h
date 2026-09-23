#pragma once
#include <SFML/Graphics.hpp>
#include "../physics/World.h"
#include "../graphics/Renderer.h"
#include "../graphics/UI.h"

class Application {
public:
    Application();
    void run();

private:
    void handleEvents();

    sf::RenderWindow window;
    World world;
    Renderer renderer;
    UI ui;
};