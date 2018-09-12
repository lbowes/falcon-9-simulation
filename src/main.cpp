#include "Simulation.h"
#include "Visualisation.h"

//#pragma warning(disable:4996)
//#include <float.h>
//unsigned int fp_control_state = _controlfp(_EM_INEXACT, _MCW_EM);

int main() {
	const std::string dataFilePath = "bin/telemetry.txt";

	Physics::Simulation sim(dataFilePath);
	sim.run();

	Visualisation visualisation(dataFilePath);
	visualisation.run();
}