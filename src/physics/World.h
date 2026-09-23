#pragma once
#include <vector>
#include "Particle.h"
#include "../chemistry/Bond.h"
#include "SpatialGrid.h"
#include "../core/Config.h"

class World {
public:
    World();

    void update(float dt);
    void addParticle(const Particle& p);
    void addBond(const Bond& b);
    void addAngle(const Angle& a);

    const std::vector<Particle>& getParticles() const { return particles; }
    const std::vector<Bond>& getBonds() const { return bonds; }

    void setTargetTemperature(float t) { targetTemperature = t; }
    float getCurrentTemperature() const;

    void buildWaterBox(int nMolecules, unsigned seed);

private:
    void computeForces();
    void integrateVerlet(float dt);
    void applyThermostat(float dt);
    void rebuildExclusion();
    bool isExcluded(int i, int j) const;

    std::vector<Particle> particles;
    std::vector<Bond> bonds;
    std::vector<Angle> angles;
    std::vector<std::uint8_t> exclusion;
    SpatialGrid grid;
    std::vector<int> neighborBuffer;
    float targetTemperature;
    bool forcesInitialized;
    bool exclusionDirty;
};