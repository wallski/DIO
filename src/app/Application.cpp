#include "Application.h"
#include "../core/Config.h"

Application::Application()
    : window(sf::VideoMode({ Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT }), "DIO - Water"),
    renderer(window)
{
    world.buildWaterBox(90, 42);
    world.setTargetTemperature(Config::DEFAULT_TEMPERATURE);
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

        float simDt = Config::STEPS_PER_FRAME * Config::TIMESTEP_PS;
        world.setTargetTemperature(ui.getTargetTemperature());
        world.update(simDt);

        window.clear(sf::Color::Black);
        renderer.draw(world);
        ui.draw(window);
        window.display();
    }
}

void Application::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        ui.handleEvent(*event, window);
    }
}