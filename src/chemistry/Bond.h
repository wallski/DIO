#pragma once

struct Bond {
    int i;
    int j;
    float r0;
    float k;
};

struct Angle {
    int i;
    int j;
    int kIdx;
    float theta0;
    float k;
};