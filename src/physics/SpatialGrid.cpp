#include "SpatialGrid.h"
#include <algorithm>

SpatialGrid::SpatialGrid(float boxW, float boxH, float cutoff)
    : boxW(boxW), boxH(boxH), cutoff(cutoff)
{
    cols = std::max(1, static_cast<int>(boxW / cutoff));
    rows = std::max(1, static_cast<int>(boxH / cutoff));
    cellW = boxW / cols;
    cellH = boxH / rows;
    cells.resize(cols * rows);
}

int SpatialGrid::cellIndex(float x, float y) const {
    int cx = static_cast<int>(x / cellW);
    int cy = static_cast<int>(y / cellH);
    if (cx < 0) cx = 0;
    if (cx >= cols) cx = cols - 1;
    if (cy < 0) cy = 0;
    if (cy >= rows) cy = rows - 1;
    return cy * cols + cx;
}

void SpatialGrid::build(const std::vector<Particle>& particles) {
    for (auto& c : cells) c.clear();
    for (size_t i = 0; i < particles.size(); ++i) {
        int ci = cellIndex(particles[i].position.x, particles[i].position.y);
        cells[ci].push_back(static_cast<int>(i));
    }
}

void SpatialGrid::getNeighbors(int idx, const std::vector<Particle>& particles,
    std::vector<int>& neighbors) const {
    neighbors.clear();
    int ci = cellIndex(particles[idx].position.x, particles[idx].position.y);
    int cx = ci % cols;
    int cy = ci / cols;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = cx + dx;
            int ny = cy + dy;
            if (nx < 0 || nx >= cols || ny < 0 || ny >= rows) continue;
            const auto& cell = cells[ny * cols + nx];
            for (int j : cell) {
                if (j != idx) neighbors.push_back(j);
            }
        }
    }
}