#include "Physics/Simulation.h"
#include "Graphics/Visualisation.h"

int main() {
    Physics::Simulation sim("../dat/output.csv");
    Graphics::Visualisation vis(sim.getSampleInterval_s(), sim.getDuration_s());

	return 0;
}