#include "Earth.h"

#include <cmath>
#include <chrono/core/ChMathematics.h>

namespace Physics {
	namespace External {

		const double
			Earth::WGS84::ecc_squared = 1.0 - pow(b, 2.0) / pow(a, 2.0),
			Earth::WGS84::ecc = sqrt(ecc_squared),
			Earth::WGS84::second_ecc = ecc / sqrt(1.0 - pow(ecc, 2.0));

		double Earth::getRadiusAtLatitude_m(double geodeticLat_degs) {
			return WGS84::a / sqrt(1.0 - WGS84::ecc_squared * pow(sin(chrono::CH_C_DEG_TO_RAD * geodeticLat_degs), 2.0));
		}

		double Earth::getPhysicalRadius_m(double geodeticLat_degs) {
			// See 'radcur(lati)' on http://www.oc.nps.edu/oc2902w/coord/geodesy.js for details
			const double 
				N = getRadiusAtLatitude_m(geodeticLat_degs),
				geodeticLat_rad = chrono::CH_C_DEG_TO_RAD * geodeticLat_degs;
			
			return sqrt(pow(N * cos(geodeticLat_rad), 2.0) + pow((1.0 - WGS84::ecc_squared) * N * sin(geodeticLat_rad), 2.0));
		}

		double Earth::geocentricToGeodeticLat(double geodeticLat_degs, double altAboveEllipsoid) {
			// Approximation by stages
			// 1st use geocentric latitude as if it's geodetic, then correct alt dependence
			double 
				N = getRadiusAtLatitude_m(geodeticLat_degs),
				geodeticLat_rad = chrono::CH_C_DEG_TO_RAD * geodeticLat_degs,
				ratio = 1 - WGS84::ecc_squared * N / (N + altAboveEllipsoid),
				tlat = tan(geodeticLat_rad) / ratio,
				geodeticLatitude = chrono::CH_C_RAD_TO_DEG * atan(tlat);

			// now use this approximation for gd-lat to get rn etc.
			N = getRadiusAtLatitude_m(geodeticLatitude);
			ratio = 1 - WGS84::ecc_squared * N / (N + altAboveEllipsoid);
			tlat = tan(geodeticLat_rad) / ratio;
			geodeticLatitude = chrono::CH_C_RAD_TO_DEG * atan(tlat);

			return geodeticLatitude;
		}

	}
}