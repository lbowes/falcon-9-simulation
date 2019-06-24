#include "Physics/Simulation.h"
#include "Graphics/Visualisation.h"

int main() {
    Physics::Simulation sim(dataFile);
    Graphics::Visualisation vis(sim.getStateHistory(), sim.getSnapshotInterval_s(), sim.getDuration());

	return 0;
}