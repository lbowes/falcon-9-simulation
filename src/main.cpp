#include "Physics/Simulation.h"
#include "Graphics/Visualisation.h"

int main() {
	Physics::Simulation sim;
	sim.run();

	Graphics::Visualisation vis(sim.getStateHistory(), sim.getSnapshotInterval_s());
	vis.run();

	return 0;
}