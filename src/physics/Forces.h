#pragma once
#include "Particle.h"

namespace Forces {
    Vec2 lennardJones(const Particle& a, const Particle& b, float epsilon, float sigma);
}