#include "Renderer.h"
#include "../core/Config.h"

Renderer::Renderer(sf::RenderWindow& window) : window(window) {}

void Renderer::draw(const World& world) {
    const auto& particles = world.getParticles();
    const float bondSq = Config::BOND_THRESHOLD * Config::BOND_THRESHOLD;

    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            Vec2 delta = particles[j].position - particles[i].position;
            if (delta.lengthSquared() < bondSq) {
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(particles[i].position.x, particles[i].position.y),
                               sf::Color(80, 80, 80)),
                    sf::Vertex(sf::Vector2f(particles[j].position.x, particles[j].position.y),
                               sf::Color(80, 80, 80))
                };
                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }
    }

    for (const auto& p : particles) {
        sf::CircleShape circle(p.radius);
        if (p.charge > 0.0f)       circle.setFillColor(sf::Color(255, 80, 80));
        else if (p.charge < 0.0f)  circle.setFillColor(sf::Color(80, 80, 255));
        else                       circle.setFillColor(sf::Color(200, 200, 200));
        circle.setOrigin(sf::Vector2f(p.radius, p.radius));
        circle.setPosition(sf::Vector2f(p.position.x, p.position.y));
        window.draw(circle);
    }
}