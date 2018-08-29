#ifndef EXTERNAL_EARTH_H
#define EXTERNAL_EARTH_H
#pragma once

#include <cmath>
#include <glm/trigonometric.hpp>

namespace External {

	class Earth {
	public:
		enum class Radius : unsigned { equatorial = 0, polar = 90 };

		struct WGS84 {
			static constexpr double a = 6378137.0;
			static constexpr double f = 1.0 / 298.257223563;
			static constexpr double b = a * (1.0 - f);
			static const double ecc_squared;
			static const double ecc;                         //Eccentricity of Earth
			static const double second_ecc;                  //Second eccentricity of Earth
		};

		static constexpr double mMass_kg = 5.9722e24;

	public:
		static double getRadiusAtLatitude_m(double geodeticLat_degrees);
		static double getPhysicalRadius_m(double geodeticLat_degrees);
		static double geocentricToGeodeticLat(double geocentricLat_degrees, double altAboveEllipsoid = 0.0);

	private:
		Earth() = delete;

	};

}

#endif
