#pragma once
#include <vector>
#include "Particle.h"

class SpatialGrid {
public:
    SpatialGrid(float boxW, float boxH, float cutoff);

    void build(const std::vector<Particle>& particles);
    void getNeighbors(int idx, const std::vector<Particle>& particles,
        std::vector<int>& neighbors) const;

private:
    int cellIndex(float x, float y) const;

    float boxW, boxH, cutoff;
    int cols, rows;
    float cellW, cellH;
    std::vector<std::vector<int>> cells;
};