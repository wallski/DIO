#pragma once
#include "Particle.h"
#include "../chemistry/Bond.h"

namespace Forces {
    Vec2 lennardJones(const Particle& a, const Particle& b);
    Vec2 coulomb(const Particle& a, const Particle& b);
    Vec2 bondForce(const Particle& a, const Particle& b, const Bond& bond);
    void angleForce(const Particle& a, const Particle& b, const Particle& c,
        const Angle& ang, Vec2& fi, Vec2& fj, Vec2& fk);
}