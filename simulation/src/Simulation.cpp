#include "Simulation.h"

#include <fstream>
#include <iomanip>


Simulation::Simulation() :
    mUpdateFreq_hz(100) {
}


void Simulation::run() {
    const double dt_s = 1.0 / mUpdateFreq_hz;

    nlohmann::json stateHistory;

    while(!stopConditionMet()) {
        //const SensorReadings readings = mFalcon9.readSensors();
        //const ControlProfile p = mFlightComputer.process(readings);
        //mFalcon9.setControlProfile(p);

        mSystem.DoStepDynamics(dt_s);

        saveSnapshotTo(stateHistory);
    }

    writeHistoryToFile(stateHistory);
}


bool Simulation::stopConditionMet() const {
    return mSystem.GetChTime() > 10.0;
}


void Simulation::saveSnapshotTo(nlohmann::json& history) const {
    // Write the state of the simulation to a json object
    history["history"] = "todo";
}


void Simulation::writeHistoryToFile(const nlohmann::json& history) const {
    // Write the saved state snapshots to an output file
    std::ofstream file("output.json", std::fstream::out);

    if(!file.is_open()) {
        printf("ERROR: Could not open output file.\n");
        return;
    }

    file << std::setw(4) << history;
    file.close();
}
