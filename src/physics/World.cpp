#include "World.h"
#include "Forces.h"
#include "../core/Config.h"

World::World() = default;

void World::addParticle(const Particle& p) {
    particles.push_back(p);
}

void World::update(float dt) {
    if (dt <= 0.0f) return;
    float subDt = dt / Config::SUBSTEPS;
    for (int i = 0; i < Config::SUBSTEPS; ++i) {
        step(subDt);
    }
}

void World::step(float dt) {
    for (auto& p : particles) {
        p.force = Vec2(0.0f, 0.0f);
    }

    for (auto& p : particles) {
        p.applyForce(Vec2(0.0f, Config::GRAVITY * p.mass));
    }

    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            Vec2 f = Forces::lennardJones(particles[i], particles[j],
                Config::LENNARD_EPSILON,
                Config::LENNARD_SIGMA);
            particles[i].applyForce(f);
            particles[j].applyForce(f * -1.0f);
        }
    }

    for (auto& p : particles) {
        Vec2 acceleration = p.force / p.mass;
        p.velocity += acceleration * dt;
        p.velocity *= Config::DAMPING;
        p.position += p.velocity * dt;

        if (p.position.y + p.radius > Config::SCREEN_HEIGHT) {
            p.position.y = Config::SCREEN_HEIGHT - p.radius;
            p.velocity.y *= -0.8f;
        }
        if (p.position.y - p.radius < 0.0f) {
            p.position.y = p.radius;
            p.velocity.y *= -0.8f;
        }
        if (p.position.x - p.radius < 0.0f) {
            p.position.x = p.radius;
            p.velocity.x *= -0.8f;
        }
        if (p.position.x + p.radius > Config::SCREEN_WIDTH) {
            p.position.x = Config::SCREEN_WIDTH - p.radius;
            p.velocity.x *= -0.8f;
        }
    }
}