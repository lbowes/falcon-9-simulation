#ifndef SIMULATION_H_
#define SIMULATION_H_


#include "../3rd_party/json.hpp"

#include <chrono/physics/ChSystemNSC.h>
#include <stdint.h>


class Simulation {
private:
    chrono::ChSystemNSC mSystem;
    const uint16_t mUpdateFreq_hz;

public:
    Simulation();
    ~Simulation() = default;

    void run();

private:
    bool stopConditionMet() const;
    void saveSnapshotTo(nlohmann::json& history) const;
    void writeHistoryToFile(const nlohmann::json& history) const;
};


#endif // SIMULATION_H_

