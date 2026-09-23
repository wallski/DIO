#include "Application.h"
#include "../core/Config.h"
#include <random>

Application::Application()
    : window(sf::VideoMode({ Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT }), "DIO - Simulation"),
    renderer(window)
{
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> xDist(80.0f, Config::SCREEN_WIDTH - 80.0f);
    std::uniform_real_distribution<float> yDist(80.0f, Config::SCREEN_HEIGHT - 80.0f);
    std::uniform_real_distribution<float> vDist(-Config::SPAWN_VELOCITY, Config::SPAWN_VELOCITY);

    for (int i = 0; i < Config::PARTICLE_COUNT; ++i) {
        float charge = (i % 2 == 0) ? 1.0f : -1.0f;
        Particle p(Vec2(xDist(rng), yDist(rng)), 1.0f,
            static_cast<float>(Config::PARTICLE_RADIUS), charge);
        p.velocity = Vec2(vDist(rng), vDist(rng));
        world.addParticle(p);
    }

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

        world.setTargetTemperature(ui.getTargetTemperature());

        world.update(dt);

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