#pragma once
#include <vector>
#include "Particle.h"


class World {
public:
	World();


	void update(float dt);
	void addParticle(const Particle& p);

	const std::vector<Particle>& getParticles() const { return particles; }

private:
	std::vector<Particle> particles;

};