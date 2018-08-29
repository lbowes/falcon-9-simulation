#include "Earth.h"

namespace External {

	const double
		Earth::WGS84::ecc_squared = 1.0 - pow(b, 2.0) / pow(a, 2.0),
		Earth::WGS84::ecc = sqrt(ecc_squared),
		Earth::WGS84::second_ecc = ecc / sqrt(1.0 - pow(ecc, 2.0));

	double Earth::getRadiusAtLatitude_m(double geodeticLat_degrees) {
		return WGS84::a / sqrt(1.0 - WGS84::ecc_squared * pow(sin(glm::radians(geodeticLat_degrees)), 2.0));
	}

	double Earth::getPhysicalRadius_m(double geodeticLat_degrees) {
		//See 'radcur(lati)' on http://www.oc.nps.edu/oc2902w/coord/geodesy.js for details
		double N = getRadiusAtLatitude_m(geodeticLat_degrees);
		return sqrt(pow(N * cos(glm::radians(geodeticLat_degrees)), 2.0) + pow((1.0 - WGS84::ecc_squared) * N * sin(glm::radians(geodeticLat_degrees)), 2.0));
	}

	double Earth::geocentricToGeodeticLat(double geocentricLat_degrees, double altAboveEllipsoid) {
		//Approximation by stages
		//1st use geocentric latitude as if it's geodetic, then correct alt dependence
		double 
			N = getRadiusAtLatitude_m(geocentricLat_degrees),
			ratio = 1 - WGS84::ecc_squared * N / (N + altAboveEllipsoid),
			tlat = tan(glm::radians(geocentricLat_degrees)) / ratio,
			geodeticLatitude = glm::degrees(atan(tlat));

		//now use this approximation for gd-lat to get rn etc.
		N = getRadiusAtLatitude_m(geodeticLatitude);
		ratio = 1 - WGS84::ecc_squared * N / (N + altAboveEllipsoid);
		tlat = tan(glm::radians(geocentricLat_degrees)) / ratio;
		geodeticLatitude = glm::degrees(atan(tlat));

		return geodeticLatitude;
	}

}