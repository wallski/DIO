#include "Renderer.h"

Renderer::Renderer(sf::RenderWindow& window) : window(window) {}
	

void Renderer::draw(const World& world) {
	window.clear(sf::Color::Black);

	for (const auto& p : world.getParticles()) {
		sf::CircleShape circle(p.radius);
		circle.setFillColor(sf::Color::Green);

		circle.setOrigin(sf::Vector2f(p.radius, p.radius));
		circle.setPosition(sf::Vector2f(p.position.x, p.position.y));

		window.draw(circle);
	}

	window.display();
}
