#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "System.h"


class Simulation {
private:
    const unsigned int mUpdateFreq_hz;
    const double mDuration_s;
    double mSimulatedTime_s;
    System mSystem;

public:
    Simulation();
    ~Simulation() = default;

    void run();

private:
    void init();
    void close();
    bool shouldTerminate();
};


#endif // SIMULATION_H_

