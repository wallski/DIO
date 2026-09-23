#include "Application.h"
#include "../core/Config.h"

Application::Application()
    : window(sf::VideoMode({ Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT }), "DIO - Simulation"),
    renderer(window)

{
    world.addParticle(Particle(Vec2(Config::SCREEN_WIDTH / 2.0f, 100.0f), 1.0f, 10.0f));
}

void Application::run() {
    sf::Clock clock;
    float lastTime = 0.0f;

    while (window.isOpen()) {
        handleEvents();

        float currentTime = clock.getElapsedTime().asSeconds();
        float dt = currentTime - lastTime;
        lastTime = currentTime;
        if (dt > 0.1f) dt = 0.1f;

        world.update(dt);
        renderer.draw(world);
    }
}

void Application::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}