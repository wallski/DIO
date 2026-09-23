#include "Renderer.h"
#include "../core/Config.h"
#include <algorithm>
#include <cstdint>

Renderer::Renderer(sf::RenderWindow& window) : window(window) {}

void Renderer::draw(const World& world) {
    const auto& particles = world.getParticles();
    const float scale = Config::PIXELS_PER_ANGSTROM;
    constexpr float BOND_CUTOFF = 4.767f;
    constexpr float BOND_CUTOFF_SQ = BOND_CUTOFF * BOND_CUTOFF;
    constexpr float V_MIN = 0.0f;
    constexpr float V_MAX = 1.2f;

    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            Vec2 delta = particles[j].position - particles[i].position;
            if (delta.lengthSquared() < BOND_CUTOFF_SQ) {
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(particles[i].position.x * scale,
                                            particles[i].position.y * scale),
                               sf::Color(70, 70, 70)),
                    sf::Vertex(sf::Vector2f(particles[j].position.x * scale,
                                            particles[j].position.y * scale),
                               sf::Color(70, 70, 70))
                };
                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }
    }

    for (const auto& p : particles) {
        float speed = p.velocity.length();
        float t = std::clamp((speed - V_MIN) / (V_MAX - V_MIN), 0.0f, 1.0f);

        sf::Color color(
            static_cast<std::uint8_t>(80 + t * 175),
            static_cast<std::uint8_t>(120 - t * 60),
            static_cast<std::uint8_t>(255 - t * 220)
        );

        sf::CircleShape circle(p.radius * scale);
        circle.setFillColor(color);
        circle.setOrigin(sf::Vector2f(p.radius * scale, p.radius * scale));
        circle.setPosition(sf::Vector2f(p.position.x * scale, p.position.y * scale));
        window.draw(circle);
    }
}