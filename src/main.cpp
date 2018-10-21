#include "Simulation.h"
#include "Visualisation.h"

//#pragma warning(disable:4996)
//#include <float.h>
//unsigned int fp_control_state = _controlfp(_EM_INEXACT, _MCW_EM);

int main() {
	Physics::Simulation sim("telemetry.txt");
	sim.run();

	Visualisation visualisation(sim.getStateHistory());
	visualisation.run();
}