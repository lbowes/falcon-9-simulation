#ifndef SIMULATION_H_
#define SIMULATION_H_


#include "../3rd_party/json.hpp"
#include "Falcon9/Falcon9_Vehicle.h"
#include "FlightComputer.h"

#include <chrono/physics/ChSystemNSC.h>
#include <stdint.h>


class Simulation {
private:
    chrono::ChSystemNSC mSystem;
    const uint16_t mUpdateFreq_hz;
    Hardware::Falcon9_Vehicle mFalcon9;
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


#endif // SIMULATION_H_

