#include "Element.h"

static const Element ELEMENTS[] = {
    { ElementType::Argon,    "Ar", 39.948f,  3.405f, 0.997f,  0.55f },
    { ElementType::Hydrogen, "H",   1.008f,  2.500f, 0.000f,  0.30f },
    { ElementType::Carbon,   "C",  12.011f,  3.400f, 0.360f,  0.45f },
    { ElementType::Nitrogen, "N",  14.007f,  3.250f, 0.711f,  0.42f },
    { ElementType::Oxygen,   "O",  15.999f,  3.1507f, 0.6364f, 0.42f },
};

const Element& getElement(ElementType t) {
    return ELEMENTS[static_cast<int>(t)];
}