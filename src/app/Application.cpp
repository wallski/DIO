#include "Application.h"
#include "../core/Config.h"
#include <random>

Application::Application()
    : window(sf::VideoMode({ Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT }), "DIO - Argon"),
    renderer(window)
{
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> xDist(4.0f, Config::BOX_WIDTH - 4.0f);
    std::uniform_real_distribution<float> yDist(4.0f, Config::BOX_HEIGHT - 4.0f);
    std::uniform_real_distribution<float> vDist(-0.3f, 0.3f);

    constexpr int COUNT = 80;
    constexpr float MIN_SEPARATION_SQ = 3.405f * 3.405f;

    int attempts = 0;
    int placed = 0;
    while (placed < COUNT && attempts < 10000) {
        ++attempts;
        Vec2 pos(xDist(rng), yDist(rng));

        bool tooClose = false;
        for (const auto& existing : world.getParticles()) {
            Vec2 d = existing.position - pos;
            if (d.lengthSquared() < MIN_SEPARATION_SQ) {
                tooClose = true;
                break;
            }
        }
        if (tooClose) continue;

        Particle p(pos, 39.95f, 0.5f, 0.0f);
        p.velocity = Vec2(vDist(rng), vDist(rng));
        world.addParticle(p);
        ++placed;
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