#pragma once
#include <cstdint>

enum class ElementType : std::uint8_t {
    Argon = 0,
    Hydrogen,
    Carbon,
    Nitrogen,
    Oxygen,
    Count
};

struct Element {
    ElementType type;
    const char* name;
    float mass;
    float sigma;
    float epsilon;
    float radius;
};

const Element& getElement(ElementType t);