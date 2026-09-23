#pragma once

namespace Config {
    constexpr int SCREEN_WIDTH = 1200;
    constexpr int SCREEN_HEIGHT = 720;

    constexpr float BOLTZMANN = 0.0083144626f;
    constexpr float COULOMB_K = 1389.35458f;

    constexpr float BOX_WIDTH = 60.0f;
    constexpr float BOX_HEIGHT = 36.0f;

    constexpr float TIMESTEP_PS = 0.0005f;
    constexpr int   STEPS_PER_FRAME = 20;

    constexpr float THERMOSTAT_TAU = 0.1f;

    constexpr float MIN_TEMPERATURE = 0.0f;
    constexpr float MAX_TEMPERATURE = 1000.0f;
    constexpr float DEFAULT_TEMPERATURE = 300.0f;

    constexpr float PIXELS_PER_ANGSTROM = 20.0f;

    constexpr float CUTOFF = 10.0f;
}