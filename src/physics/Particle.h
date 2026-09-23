#pragma once
#include "../core/Math.h"
#include "../chemistry/Element.h"

struct Particle {
    Vec2 position;
    Vec2 velocity;
    Vec2 force;
    ElementType element;
    float charge;
    int moleculeId;

    Particle(Vec2 pos, ElementType e, float q, int molId)
        : position(pos), velocity(0.0f, 0.0f), force(0.0f, 0.0f),
        element(e), charge(q), moleculeId(molId) {}

    void applyForce(const Vec2& f) { force += f; }

    float mass()    const { return getElement(element).mass; }
    float sigma()   const { return getElement(element).sigma; }
    float epsilon() const { return getElement(element).epsilon; }
    float radius()  const { return getElement(element).radius; }
};