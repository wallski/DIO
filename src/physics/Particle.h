#pragma once
#include "../core/Math.h"

struct Particle {
	Vec2 position;
	Vec2 velocity;
	Vec2 force;
	float mass;
	float radius;

	Particle(Vec2 pos, float m, float r) 
		: position(pos), velocity(0.0f, 0.0f), force(0.0f, 0.0f), mass(m), radius(r) {}

	void applyForce(const Vec2& f) {
		force += f;
	}
};