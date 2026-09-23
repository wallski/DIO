#include "World.h"
#include "Forces.h"
#include "../core/Config.h"
#include <cmath>
#include <random>

namespace {
    constexpr float AMU_A2_PS2_TO_KJMOL = 1.0f;
    constexpr float MAX_VELOCITY = 8.0f;
}

World::World()
    : grid(Config::BOX_WIDTH, Config::BOX_HEIGHT, Config::CUTOFF),
    targetTemperature(Config::DEFAULT_TEMPERATURE),
    forcesInitialized(false),
    exclusionDirty(true)
{}

void World::addParticle(const Particle& p) {
    particles.push_back(p);
    exclusionDirty = true;
}

void World::addBond(const Bond& b) {
    bonds.push_back(b);
    exclusionDirty = true;
}

void World::addAngle(const Angle& a) {
    angles.push_back(a);
}

void World::rebuildExclusion() {
    size_t n = particles.size();
    exclusion.assign(n * n, 0);
    for (const auto& b : bonds) {
        exclusion[b.i * n + b.j] = 1;
        exclusion[b.j * n + b.i] = 1;
    }
    exclusionDirty = false;
}

bool World::isExcluded(int i, int j) const {
    size_t n = particles.size();
    return exclusion[i * n + j] != 0;
}

float World::getCurrentTemperature() const {
    if (particles.empty()) return 0.0f;

    float sumKE = 0.0f;
    for (const auto& p : particles) {
        sumKE += 0.5f * p.mass() * p.velocity.lengthSquared();
    }

    float keKJmol = sumKE * AMU_A2_PS2_TO_KJMOL;
    float nDf = 2.0f * static_cast<float>(particles.size());
    return (2.0f * keKJmol) / (nDf * Config::BOLTZMANN);
}

void World::update(float dt) {
    if (dt <= 0.0f) return;

    if (!forcesInitialized) {
        computeForces();
        forcesInitialized = true;
    }

    float subDt = dt / Config::STEPS_PER_FRAME;
    for (int i = 0; i < Config::STEPS_PER_FRAME; ++i) {
        integrateVerlet(subDt);
        applyThermostat(subDt);
    }
}

void World::computeForces() {
    if (exclusionDirty) rebuildExclusion();

    for (auto& p : particles) p.force = Vec2(0.0f, 0.0f);

    grid.build(particles);

    for (size_t i = 0; i < particles.size(); ++i) {
        grid.getNeighbors(static_cast<int>(i), particles, neighborBuffer);
        for (int j : neighborBuffer) {
            if (j <= static_cast<int>(i)) continue;
            if (isExcluded(static_cast<int>(i), j)) continue;

            Vec2 f = Forces::lennardJones(particles[i], particles[j]);
            f += Forces::coulomb(particles[i], particles[j]);

            particles[i].applyForce(f);
            particles[j].applyForce(f * -1.0f);
        }
    }

    for (const auto& b : bonds) {
        Vec2 f = Forces::bondForce(particles[b.i], particles[b.j], b);
        particles[b.i].applyForce(f);
        particles[b.j].applyForce(f * -1.0f);
    }

    for (const auto& ang : angles) {
        Vec2 fi, fj, fk;
        Forces::angleForce(particles[ang.i], particles[ang.j], particles[ang.k],
            ang, fi, fj, fk);
        particles[ang.i].applyForce(fi);
        particles[ang.j].applyForce(fj);
        particles[ang.k].applyForce(fk);
    }
}

void World::integrateVerlet(float dt) {
    float halfDt = 0.5f * dt;

    for (auto& p : particles) {
        Vec2 a = p.force / p.mass();
        p.velocity += a * halfDt;
        p.position += p.velocity * dt;

        if (p.position.x < 0.0f) { p.position.x = -p.position.x;                            p.velocity.x = -p.velocity.x; }
        if (p.position.x > Config::BOX_WIDTH) { p.position.x = 2.0f * Config::BOX_WIDTH - p.position.x;  p.velocity.x = -p.velocity.x; }
        if (p.position.y < 0.0f) { p.position.y = -p.position.y;                            p.velocity.y = -p.velocity.y; }
        if (p.position.y > Config::BOX_HEIGHT) { p.position.y = 2.0f * Config::BOX_HEIGHT - p.position.y; p.velocity.y = -p.velocity.y; }
    }

    computeForces();

    for (auto& p : particles) {
        Vec2 a = p.force / p.mass();
        p.velocity += a * halfDt;

        float v2 = p.velocity.lengthSquared();
        if (v2 > MAX_VELOCITY * MAX_VELOCITY) {
            p.velocity = p.velocity.normalized() * MAX_VELOCITY;
        }
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

void World::buildWaterBox(int nMolecules, unsigned seed) {
    particles.clear();
    bonds.clear();
    angles.clear();
    exclusionDirty = true;

    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> xDist(4.0f, Config::BOX_WIDTH - 4.0f);
    std::uniform_real_distribution<float> yDist(4.0f, Config::BOX_HEIGHT - 4.0f);
    std::uniform_real_distribution<float> angDist(0.0f, 6.2831853f);
    std::uniform_real_distribution<float> vDist(-0.15f, 0.15f);

    constexpr float R_OH = 0.9572f;
    constexpr float THETA_HOH = 1.8242f;
    constexpr float KB_OH = 3000.0f;
    constexpr float KA_HOH = 300.0f;

    const float h2x = R_OH * std::cos(THETA_HOH);
    const float h2y = R_OH * std::sin(THETA_HOH);

    constexpr float MIN_OO_SQ = 2.6f * 2.6f;

    int placed = 0;
    int attempts = 0;
    while (placed < nMolecules && attempts < 200000) {
        ++attempts;
        float cx = xDist(rng);
        float cy = yDist(rng);

        bool tooClose = false;
        for (const auto& p : particles) {
            if (p.element != ElementType::Oxygen) continue;
            float dx = p.position.x - cx;
            float dy = p.position.y - cy;
            if (dx * dx + dy * dy < MIN_OO_SQ) { tooClose = true; break; }
        }
        if (tooClose) continue;

        float phi = angDist(rng);
        float ca = std::cos(phi);
        float sa = std::sin(phi);

        Vec2 oPos(cx, cy);
        Vec2 h1Pos(cx + R_OH * ca, cy + R_OH * sa);
        Vec2 h2Pos(cx + h2x * ca - h2y * sa, cy + h2x * sa + h2y * ca);

        int base = static_cast<int>(particles.size());
        int molId = placed;

        particles.emplace_back(oPos, ElementType::Oxygen, -0.834f, molId);
        particles.emplace_back(h1Pos, ElementType::Hydrogen, 0.417f, molId);
        particles.emplace_back(h2Pos, ElementType::Hydrogen, 0.417f, molId);

        particles[base].velocity = Vec2(vDist(rng), vDist(rng));
        particles[base + 1].velocity = Vec2(vDist(rng), vDist(rng));
        particles[base + 2].velocity = Vec2(vDist(rng), vDist(rng));

        bonds.push_back({ base, base + 1, R_OH, KB_OH });
        bonds.push_back({ base, base + 2, R_OH, KB_OH });
        angles.push_back({ base + 1, base, base + 2, THETA_HOH, KA_HOH });

        ++placed;
    }

    forcesInitialized = false;
}