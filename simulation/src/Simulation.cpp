#include "Simulation.h"

#include <fstream>
#include <iomanip>


namespace F9Sim {
namespace Physics {


Simulation::Simulation() :
    mUpdateFreq_hz(200),
    mFalcon9(mSystem) {

    //temp
    chrono::collision::ChCollisionModel::SetDefaultSuggestedEnvelope(1.0);
    chrono::collision::ChCollisionModel::SetDefaultSuggestedMargin(0.05);

    mSystem.SetSolverType(chrono::ChSolver::Type::BARZILAIBORWEIN);
    mSystem.SetTimestepperType(chrono::ChTimestepper::Type::EULER_IMPLICIT_PROJECTED);
    mSystem.SetSolverMaxIterations(20);

#define GRAVITY 0
#if GRAVITY
    mSystem.Set_G_acc(chrono::ChVector<>(0.0, -9.80665, 0.0));
#else
    mSystem.Set_G_acc(chrono::ChVector<>(0.0, 0.0, 0.0));
#endif
}


void Simulation::run() {
    const double dt_s = 1.0 / mUpdateFreq_hz;

    nlohmann::json output;
    output["history"] = nlohmann::json::array();

    while(!stopConditionMet()) {
        const SensorReadings readings = mFalcon9.readSensors();
        const ControlProfile p = mFlightComputer.process(readings);
        mFalcon9.setControlProfile(p);

        saveSnapshotTo(output["history"]);

        mSystem.DoStepDynamics(dt_s);
    }

    writeOutputToFile(output);
}


bool Simulation::stopConditionMet() const {
    return mSystem.GetChTime() > 10.0;
}


void Simulation::saveSnapshotTo(nlohmann::json& history) const {
    nlohmann::json snapshot;

    snapshot["time_s"] = mSystem.GetChTime();
    mFalcon9.saveSnapshotTo(snapshot["system"]);
    // todo: save the complete contents of a simulation snapshot to json object

    history.push_back(snapshot);
}


void Simulation::writeOutputToFile(const nlohmann::json& history) const {
    std::ofstream file("output.json", std::fstream::out);

    if(!file.is_open()) {
        printf("ERROR: Could not open output file.\n");
        return;
    }

    file << std::setw(4) << history;
    file.close();
}


} // namespace Physics
} // namespace F9Sim
