#include "GeoCoordUtils.h"
#include "Earth.h"

#include <chrono/core/ChMathematics.h>

namespace External {

	chrono::Vector GeoCoordUtils::toGeodetic(const chrono::ChVector<>& ecef_XYZ) 
		// Converts an ECEF position vector (X, Y, Z) to a geodetic coordinate (latitude, longitude, height)
		// Implementation of https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#The_application_of_Ferrari's_solution
		// Tested using:
		//  - http://www.oc.nps.edu/oc2902w/coord/llhxyz.htm
		//  - http://www.sysense.com/products/ecef_lla_converter/index.html
		//  - https://tool-online.com/en/coordinate-converter.php
	{
		using namespace Physics::External;
		
		const double
			X = ecef_XYZ.x(),
			Y = ecef_XYZ.z(),
			Z = ecef_XYZ.y(),
			a = Earth::WGS84::a,
			b = Earth::WGS84::b,
			e = Earth::WGS84::ecc,
			e_squared = Earth::WGS84::ecc_squared,
			e_prime = Earth::WGS84::second_ecc;

		const double
			r               = sqrt(pow(X, 2) + pow(Y, 2)),
			e_prime_squared = (pow(a, 2) - pow(b, 2)) / pow(b, 2),
			E_squared       = pow(a, 2) - pow(b, 2),
			F               = 54 * pow(b, 2) * pow(Z, 2),
			G               = pow(r, 2) + (1 - e_squared)  * pow(Z, 2) - e_squared * E_squared,
			C               = (pow(e, 4) * F * pow(r, 2)) / pow(G, 3),
			S               = cbrt(1 + C + sqrt(pow(C, 2) + 2 * C)),
			P               = F / (3 * pow(S + 1 / S + 1, 2) * pow(G, 2)),
			Q               = sqrt(1 + 2 * pow(e, 4) * P),
			r0              = (-(P * e_squared * r) / (1 + Q)) + sqrt((0.5 * pow(a, 2) * (1 + 1 / Q)) - ((P * (1 - e_squared) * pow(Z, 2)) / (Q * (1 + Q))) - (0.5 * P * pow(r, 2))),
			U               = sqrt(pow(r - e_squared * r0, 2) + pow(Z, 2)),
			V               = sqrt(pow(r - e_squared * r0, 2) + (1 - e_squared) * pow(Z, 2)),
			Z0              = (pow(b, 2) * Z) / (a * V),
			height          = U * (1 - pow(b, 2) / (a * V)),
			latitude        = chrono::CH_C_DEG_TO_RAD * atan((Z + pow(e_prime, 2) * Z0) / r),
			longitude       = chrono::CH_C_DEG_TO_RAD * atan2(Y, X);

		return chrono::Vector(latitude, longitude, height);
	}

	chrono::Vector GeoCoordUtils::toECEF(const chrono::ChVector<>& geodetic_LLH) 
		// Converts a geodetic coordinate (latitude, longitude, height) to an ECEF position vector (X, Y, Z)
		// Implementation of https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#From_geodetic_to_ECEF_coordinates 
	{
		const double
			lat = chrono::CH_C_DEG_TO_RAD * geodetic_LLH.x(),
			lon = chrono::CH_C_DEG_TO_RAD * geodetic_LLH.y(),
			N = Physics::External::Earth::getRadiusAtLatitude_m(geodetic_LLH.x()),
			x = (N + geodetic_LLH.z()) * cos(lat) * cos(lon),
			z = (N + geodetic_LLH.z()) * cos(lat) * sin(lon),
			y = ((1.0 - pow(Physics::External::Earth::WGS84::ecc, 2.0)) * N + geodetic_LLH.z()) * sin(lat);

		return chrono::Vector(x, y, z);
	}

}