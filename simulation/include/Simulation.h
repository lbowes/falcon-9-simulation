#ifndef F9SIM_PHYSICS_SIMULATION_H_
#define F9SIM_PHYSICS_SIMULATION_H_
#pragma once


#include "../3rd_party/json.hpp"
#include "Falcon9/Falcon9_Vehicle.h"
#include "FlightComputer.h"

#include <chrono/physics/ChSystemNSC.h>
#include <stdint.h>


namespace F9Sim {
namespace Physics {


class Simulation {
private:
    chrono::ChSystemNSC mSystem;
    const uint16_t mUpdateFreq_hz;
    Falcon9_Vehicle mFalcon9;
    Software::FlightComputer mFlightComputer;

public:
    Simulation();
    ~Simulation() = default;

    void run();

private:
    bool stopConditionMet() const;
    void saveSnapshotTo(nlohmann::json& history) const;
    void writeOutputToFile(const nlohmann::json& history) const;
};


} // namespace Physics
} // namespace F9Sim


#endif // F9SIM_PHYSICS_SIMULATION_H_

