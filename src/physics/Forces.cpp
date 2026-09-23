#include "Forces.h"
#include "../core/Config.h"
#include <cmath>

Vec2 Forces::lennardJones(const Particle& a, const Particle& b, float epsilon, float sigma) {
    Vec2 delta = a.position - b.position;

    float r2 = delta.lengthSquared();
    if (r2 < 0.01f) return Vec2(0.0f, 0.0f);

    float r = std::sqrt(r2);
    float sr = sigma / r;
    float sr6 = sr * sr * sr * sr * sr * sr;
    float sr12 = sr6 * sr6;

    float magnitude = (24.0f * epsilon / r) * (2.0f * sr12 - sr6);

    constexpr float MAX_LJ_FORCE = 500.0f;
    if (magnitude > MAX_LJ_FORCE) magnitude = MAX_LJ_FORCE;
    if (magnitude < -MAX_LJ_FORCE) magnitude = -MAX_LJ_FORCE;

    Vec2 dir = delta / r;
    return dir * magnitude;
}