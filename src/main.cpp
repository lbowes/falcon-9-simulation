#include "F9Simulation.h"

//#pragma warning(disable:4996)
//#include <float.h>
//unsigned int fp_control_state = _controlfp(_EM_INEXACT, _MCW_EM);

int main() {
	F9Simulation sim;
	sim.run();

	//glm::dvec3 gpsCoord = { 0.0, 0.0, 0.0 };
	//Physics::External::SurfaceLocation location = { "test", gpsCoord };
//
	//glm::dvec3 pos_ecef = location.getEUN_to_ECEFTransform().toParentSpace(glm::dvec3(0.0, 200.0, 0.0));
	//printf("pos_ecef: %f, %f, %f\n", pos_ecef.x, pos_ecef.y, pos_ecef.z);
}