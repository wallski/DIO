#include "World.h"
#include "Forces.h"
#include "../core/Config.h"
#include <cmath>

namespace {
    constexpr float EPSILON = 0.997f;
    constexpr float SIGMA = 3.405f;
    constexpr float AMU_A2_PS2_TO_KJMOL = 1.0f;
    constexpr float MAX_VELOCITY = 5.0f;
}

World::World() = default;

void World::addParticle(const Particle& p) {
    particles.push_back(p);
}

float World::getCurrentTemperature() const {
    if (particles.empty()) return 0.0f;

    float sumKE = 0.0f;
    for (const auto& p : particles) {
        sumKE += 0.5f * p.mass * p.velocity.lengthSquared();
    }

    float keKJmol = sumKE * AMU_A2_PS2_TO_KJMOL;
    float nDf = 2.0f * static_cast<float>(particles.size());
    return (2.0f * keKJmol) / (nDf * Config::BOLTZMANN);
}

void World::update(float dt) {
    if (dt <= 0.0f) return;
    float subDt = dt / Config::STEPS_PER_FRAME;
    for (int i = 0; i < Config::STEPS_PER_FRAME; ++i) {
        step(subDt);
        applyThermostat(subDt);
    }
}

void World::applyThermostat(float dt) {
    if (particles.empty()) return;

    float currentT = getCurrentTemperature();
    if (currentT < 1e-6f) return;

    float lambda = std::sqrt(1.0f + (dt / Config::THERMOSTAT_TAU) *
        (targetTemperature / currentT - 1.0f));

    for (auto& p : particles) {
        p.velocity *= lambda;
    }
}

void World::step(float dt) {
    for (auto& p : particles) {
        p.force = Vec2(0.0f, 0.0f);
    }

    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            Vec2 f = Forces::lennardJones(particles[i], particles[j], EPSILON, SIGMA);
            particles[i].applyForce(f);
            particles[j].applyForce(f * -1.0f);
        }
    }

    for (auto& p : particles) {
        Vec2 acceleration = p.force / p.mass;
        p.velocity += acceleration * dt;

        float v2 = p.velocity.lengthSquared();
        if (v2 > MAX_VELOCITY * MAX_VELOCITY) {
            p.velocity = p.velocity.normalized() * MAX_VELOCITY;
        }

        p.position += p.velocity * dt;

        if (p.position.x < 0.0f) { p.position.x = 0.0f;               p.velocity.x *= -1.0f; }
        if (p.position.x > Config::BOX_WIDTH) { p.position.x = Config::BOX_WIDTH;  p.velocity.x *= -1.0f; }
        if (p.position.y < 0.0f) { p.position.y = 0.0f;               p.velocity.y *= -1.0f; }
        if (p.position.y > Config::BOX_HEIGHT) { p.position.y = Config::BOX_HEIGHT; p.velocity.y *= -1.0f; }
    }
}