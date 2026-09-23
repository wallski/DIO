#include "World.h"
#include <iostream>
#include "../core/Config.h"

World::World() = default;

void World::addParticle(const Particle& p) {
	particles.push_back(p);
}

void World::update(float dt) {


	for (auto& p : particles) {
		p.applyForce(Vec2(0.0f, Config::GRAVITY * p.mass));

		Vec2 acceleration = p.force / p.mass;

		p.velocity += acceleration * dt;
		p.position += p.velocity * dt;

		p.force = Vec2(0.0f, 0.0f);
	
		if (p.position.y + p.radius > Config::SCREEN_HEIGHT) {
			p.position.y = Config::SCREEN_HEIGHT - p.radius;
			p.velocity.y *= -0.8f;
		}
	}
}