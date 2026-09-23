#pragma once
#include <vector>
#include "Particle.h"
#include "../core/Config.h"

class World {
public:
    World();

    void update(float dt);
    void addParticle(const Particle& p);

    const std::vector<Particle>& getParticles() const { return particles; }

    void setTargetTemperature(float t) { targetTemperature = t; }
    float getCurrentTemperature() const;

private:
    void step(float dt);
    void applyThermostat(float dt);

    std::vector<Particle> particles;
    float targetTemperature = Config::DEFAULT_TEMPERATURE;
};