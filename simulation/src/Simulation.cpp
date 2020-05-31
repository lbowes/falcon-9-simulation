#include "Simulation.h"


Simulation::Simulation() :
    mUpdateFreq_hz(100),
    mDuration_s(10.0) {

    init();
    run();
    close();
}


void Simulation::init() {
    // TODO: Open output data file
}


void Simulation::close() {
    // TODO: Close output data file
}


void Simulation::run() {
    while(!shouldTerminate()) {
        const double dt = 1.0 / mUpdateFreq_hz;
        mSystem.advanceTimeBy(dt);

        mSystem.serialiseState();

        mSimulatedTime_s += dt;
    }
}


bool Simulation::shouldTerminate() {
    return mSimulatedTime_s > mDuration_s;
}
