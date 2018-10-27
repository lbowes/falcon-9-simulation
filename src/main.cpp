#include "Simulation.h"
#include "Visualisation.h"

//#pragma warning(disable:4996)
//#include <float.h>
//unsigned int fp_control_state = _controlfp(_EM_INEXACT, _MCW_EM);

//Catch the NaNs
#include <fenv.h>
#include <float.h>
unsigned int test = feenableexcept(FE_INVALID | FE_OVERFLOW);

int main() {
	Physics::Simulation sim("telemetry.txt");
	//sim.run();

	Visualisation visualisation(sim.getStateHistory());
	visualisation.run();
}