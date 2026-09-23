#pragma once

namespace Config {
    constexpr int SCREEN_WIDTH = 1200;
    constexpr int SCREEN_HEIGHT = 720;

    constexpr float DT = 1.0f / 60.0f;
    constexpr int SUBSTEPS = 8;

    constexpr float GRAVITY = 0.0f;

    constexpr float LENNARD_EPSILON = 5000.0f;
    constexpr float LENNARD_SIGMA = 25.0f;

    constexpr float MAX_FORCE = 200000.0f;
    constexpr float DAMPING = 0.9998f;

    constexpr int PARTICLE_COUNT = 50;
    constexpr int PARTICLE_RADIUS = 5;
    constexpr float SPAWN_VELOCITY = 80.0f;

    constexpr float BOND_THRESHOLD = LENNARD_SIGMA * 1.4f;

    constexpr float MIN_TEMPERATURE = 0.0f;
    constexpr float MAX_TEMPERATURE = 8000.0f;
    constexpr float DEFAULT_TEMPERATURE = 500.0f;
    constexpr float THERMOSTAT_TAU = 0.3f;
}