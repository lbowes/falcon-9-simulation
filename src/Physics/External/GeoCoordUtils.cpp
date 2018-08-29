#include "GeoCoordUtils.h"

namespace External {

	glm::dvec3 GeoCoordUtils::toGeodetic(const glm::dvec3& ecef_XYZ) 
		//Converts an ECEF position vector (X, Y, Z) to a geodetic coordinate (latitude, longitude, height)
		//Implementation of https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#The_application_of_Ferrari's_solution
		//Checked using:
		// - http://www.oc.nps.edu/oc2902w/coord/llhxyz.htm
		// - http://www.sysense.com/products/ecef_lla_converter/index.html
		// - https://tool-online.com/en/coordinate-converter.php
	{
		const double
			X = ecef_XYZ.x,
			Y = ecef_XYZ.z,
			Z = ecef_XYZ.y,
			a = Earth::WGS84::a,
			b = Earth::WGS84::b,
			e = Earth::WGS84::ecc,
			e_squared = Earth::WGS84::ecc_squared,
			e_prime = Earth::WGS84::second_ecc;

		const double
			r = sqrt(pow(X, 2) + pow(Y, 2)),
			e_prime_squared = (pow(a, 2) - pow(b, 2)) / pow(b, 2),
			E_squared = pow(a, 2) - pow(b, 2),
			F = 54 * pow(b, 2) * pow(Z, 2),
			G = pow(r, 2) + (1 - e_squared)  * pow(Z, 2) - e_squared * E_squared,
			C = (pow(e, 4) * F * pow(r, 2)) / pow(G, 3),
			S = cbrt(1 + C + sqrt(pow(C, 2) + 2 * C)),
			P = F / (3 * pow(S + 1 / S + 1, 2) * pow(G, 2)),
			Q = sqrt(1 + 2 * pow(e, 4) * P),
			r0 = (-(P * e_squared * r) / (1 + Q)) + sqrt((0.5 * pow(a, 2) * (1 + 1 / Q)) - ((P * (1 - e_squared) * pow(Z, 2)) / (Q * (1 + Q))) - (0.5 * P * pow(r, 2))),
			U = sqrt(pow(r - e_squared * r0, 2) + pow(Z, 2)),
			V = sqrt(pow(r - e_squared * r0, 2) + (1 - e_squared) * pow(Z, 2)),
			Z0 = (pow(b, 2) * Z) / (a * V),
			height = U * (1 - pow(b, 2) / (a * V)),
			latitude = glm::degrees(atan((Z + pow(e_prime, 2) * Z0) / r)),
			longitude = glm::degrees(atan2(Y, X));

		return glm::dvec3(latitude, longitude, height);
	}

	glm::dvec3 GeoCoordUtils::toECEF(const glm::dvec3& geodetic_LLH) 
		//Converts a geodetic coordinate (latitude, longitude, height) to an ECEF position vector (X, Y, Z)
		//Implementation of https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#From_geodetic_to_ECEF_coordinates 
	{
		const double
			lat = glm::radians(geodetic_LLH.x),
			lon = glm::radians(geodetic_LLH.y),
			N = Earth::getRadiusAtLatitude_m(geodetic_LLH.x),
			x = (N + geodetic_LLH.z) * cos(lat) * cos(lon),
			z = (N + geodetic_LLH.z) * cos(lat) * sin(lon),
			y = ((1.0 - pow(Earth::WGS84::ecc, 2.0)) * N + geodetic_LLH.z) * sin(lat);

		return glm::dvec3(x, y, z);
	}

}