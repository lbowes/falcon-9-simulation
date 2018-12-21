#include "Simulation.h"
#include "Visualisation.h"

//Catch the NaNs
//#include <fenv.h>
//#include <float.h>
//unsigned int test = feenableexcept(FE_INVALID | FE_OVERFLOW);

#if 0
std::pair<double, double> gimbalTo(double clockDirAngle, double angle) 
	//Responsible for taking in gimbal parameters and converting these into correct TVC actuator lengths 
{
	//Clamp angle between 0.0 and 360.0
	clockDirAngle = fmod(clockDirAngle, 360.0);
	if(clockDirAngle < 0.0)
	    clockDirAngle += 360.0;

	const double gimbalFractionOfMax = ((angle + 6.0) / (2 * 6.0));
	
	//Handle all edge cases for tan	
	if(clockDirAngle == 0.0 || clockDirAngle == 360.0 || clockDirAngle == 180.0)
		return std::pair<double, double>(gimbalFractionOfMax, 0.5);
	else if(clockDirAngle == 90.0 || clockDirAngle == 270.0)
		return std::pair<double, double>(0.5, gimbalFractionOfMax);
	else {
		//Handle normal cases
		const double 
			absTanAlpha = abs(tan(glm::radians(clockDirAngle))),
			absOneOverTanAlpha = abs(1.0 / absTanAlpha);

		double 
			max = std::max(absTanAlpha, absOneOverTanAlpha),
			x = (max == 0.0) ? 0.0 : (absOneOverTanAlpha / max),
			z = (max == 0.0) ? 0.0 : (absTanAlpha / max);

		if(clockDirAngle > 90.0 && clockDirAngle < 270.0)
			x = -x;
 
		if(clockDirAngle > 180.0 && clockDirAngle < 360.0)
			z = -z;

		x = ((x + 1.0) / 2.0) * gimbalFractionOfMax;
		z = ((z + 1.0) / 2.0) * gimbalFractionOfMax;

		return std::pair<double, double>(x, z);
	}
}
#endif

double calcAlpha(double xAngle, double zAngle) {
	glm::dvec3 pointer = glm::vec3(0.0, -1.0, 0.0);

	//rotate by x
	pointer = glm::rotate(pointer, glm::radians(xAngle), glm::dvec3(0.0, 0.0, 1.0));

	//rotate by z
	pointer = glm::rotate(pointer, glm::radians(zAngle), glm::dvec3(1.0, 0.0, 0.0));

	return glm::degrees(glm::angle(pointer, glm::dvec3(0.0, -1.0, 0.0)));
}

int main() {
	//for(double i = 0.0; i <= 90.0; i += 1.0) {
	//	const double d = static_cast<double>(i);
	//	printf("%f, %f\n", d / 90.0, calcAlpha(d, d) / 90.0);
	//	//printf("(%f, %f)\n", d, d / calcAlpha(d, d));
	//}

	//std::cin.get();



	Physics::Simulation sim("telemetry.txt");
	sim.run();

	Visualisation visualisation(sim.getStateHistory(), sim.getSnapshotInterval());
	visualisation.run();
}
