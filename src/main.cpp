#include "simulation/Simulation.h"
#include "visualisation/Visualisation.h"

int main() {
    Simulation sim;
    sim.run();

    Visualisation vis;
    vis.run();

    return 0;
}
