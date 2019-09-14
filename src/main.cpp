#include "physics/Simulation.h"
#include "graphics/Visualisation.h"

int main() {
    const std::string dataFile_CSV = "../dat/output.csv";
    Physics::Simulation sim(dataFile_CSV);
    Graphics::Visualisation vis(dataFile_CSV, sim.getSampleInterval_s(), sim.getDuration_s());

	return 0;
}