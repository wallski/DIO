#include "Forces.h"
#include "../core/Config.h"
#include <cmath>

Vec2 Forces::lennardJones(const Particle& a, const Particle& b) {
    float sigma = 0.5f * (a.sigma() + b.sigma());
    float epsilon = std::sqrt(a.epsilon() * b.epsilon());
    if (epsilon < 1e-6f) return Vec2(0.0f, 0.0f);

    Vec2 delta = a.position - b.position;
    float r2 = delta.lengthSquared();
    if (r2 < 0.01f) return Vec2(0.0f, 0.0f);

    float r = std::sqrt(r2);
    float sr = sigma / r;
    float sr6 = sr * sr * sr * sr * sr * sr;
    float sr12 = sr6 * sr6;

    float magnitude = (24.0f * epsilon / r) * (2.0f * sr12 - sr6);

    constexpr float MAX_LJ = 500.0f;
    if (magnitude > MAX_LJ) magnitude = MAX_LJ;
    if (magnitude < -MAX_LJ) magnitude = -MAX_LJ;

    return (delta / r) * magnitude;
}

Vec2 Forces::coulomb(const Particle& a, const Particle& b) {
    if (a.charge == 0.0f || b.charge == 0.0f) return Vec2(0.0f, 0.0f);

    Vec2 delta = a.position - b.position;
    float r2 = delta.lengthSquared() + 1.0f;
    float r = std::sqrt(r2);

    float magnitude = Config::COULOMB_K * a.charge * b.charge / r2;

    constexpr float MAX_C = 500.0f;
    if (magnitude > MAX_C) magnitude = MAX_C;
    if (magnitude < -MAX_C) magnitude = -MAX_C;

    return (delta / r) * magnitude;
}

Vec2 Forces::bondForce(const Particle& a, const Particle& b, const Bond& bond) {
    Vec2 delta = a.position - b.position;
    float r = delta.length();
    if (r < 1e-6f) return Vec2(0.0f, 0.0f);

    float magnitude = -2.0f * bond.k * (r - bond.r0);
    return (delta / r) * magnitude;
}

void Forces::angleForce(const Particle& a, const Particle& b, const Particle& c,
    const Angle& ang, Vec2& fi, Vec2& fj, Vec2& fk) {
    Vec2 ba = a.position - b.position;
    Vec2 bc = c.position - b.position;
    float ra = ba.length();
    float rc = bc.length();
    if (ra < 1e-6f || rc < 1e-6f) { fi = fj = fk = Vec2(0.0f, 0.0f); return; }

    Vec2 ua = ba / ra;
    Vec2 uc = bc / rc;

    float cosTheta = ua.x * uc.x + ua.y * uc.y;
    if (cosTheta > 1.0f) cosTheta = 1.0f;
    if (cosTheta < -1.0f) cosTheta = -1.0f;
    float theta = std::acos(cosTheta);
    float sinTheta = std::sin(theta);
    if (sinTheta < 1e-6f) { fi = fj = fk = Vec2(0.0f, 0.0f); return; }

    float coef = -2.0f * ang.k * (theta - ang.theta0);
    fi = (uc - ua * cosTheta) * (coef / (ra * sinTheta));
    fk = (ua - uc * cosTheta) * (coef / (rc * sinTheta));
    fj = (fi + fk) * -1.0f;
}