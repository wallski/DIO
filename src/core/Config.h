#pragma once

namespace Config {
    constexpr int SCREEN_WIDTH = 1200;
    constexpr int SCREEN_HEIGHT = 720;

    constexpr float DT = 1.0f / 60.0f;
    constexpr int SUBSTEPS = 8;

    constexpr float GRAVITY = 0.0f;

    constexpr float LENNARD_EPSILON = 5000.0f;
    constexpr float LENNARD_SIGMA = 60.0f;

    constexpr float MAX_FORCE = 200000.0f;
    constexpr float DAMPING = 0.999f;
}