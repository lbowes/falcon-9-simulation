#include "Simulation.h"
#include "Visualisation.h"

//Catch the NaNs
//#include <fenv.h>
//#include <float.h>
//unsigned int test = feenableexcept(FE_INVALID | FE_OVERFLOW);

int main() {
	Physics::Simulation sim("telemetry.txt");
	sim.run();

	Visualisation visualisation(sim.getStateHistory(), sim.getSnapshotInterval());
	visualisation.run();
}
