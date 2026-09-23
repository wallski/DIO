#include "Renderer.h"
#include "../core/Config.h"

namespace {
    sf::Color elementColor(ElementType t) {
        switch (t) {
        case ElementType::Argon:    return sf::Color(180, 180, 200);
        case ElementType::Hydrogen: return sf::Color(240, 240, 240);
        case ElementType::Carbon:   return sf::Color(60, 60, 60);
        case ElementType::Nitrogen: return sf::Color(80, 130, 255);
        case ElementType::Oxygen:   return sf::Color(230, 60, 60);
        default:                    return sf::Color(200, 200, 200);
        }
    }
}

Renderer::Renderer(sf::RenderWindow& window) : window(window) {}

void Renderer::draw(const World& world) {
    const auto& particles = world.getParticles();
    const auto& bonds = world.getBonds();
    const float scale = Config::PIXELS_PER_ANGSTROM;

    for (const auto& b : bonds) {
        const auto& a1 = particles[b.i];
        const auto& a2 = particles[b.j];
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(a1.position.x * scale, a1.position.y * scale),
                       sf::Color(90, 90, 90)),
            sf::Vertex(sf::Vector2f(a2.position.x * scale, a2.position.y * scale),
                       sf::Color(90, 90, 90))
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    for (const auto& p : particles) {
        float r = p.radius() * scale;
        sf::CircleShape circle(r);
        circle.setFillColor(elementColor(p.element));
        circle.setOrigin(sf::Vector2f(r, r));
        circle.setPosition(sf::Vector2f(p.position.x * scale, p.position.y * scale));
        window.draw(circle);
    }
}