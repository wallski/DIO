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

    if (magnitude > Config::MAX_FORCE) magnitude = Config::MAX_FORCE;
    if (magnitude < -Config::MAX_FORCE) magnitude = -Config::MAX_FORCE;

    Vec2 dir = delta / r;
    return dir * magnitude;
}