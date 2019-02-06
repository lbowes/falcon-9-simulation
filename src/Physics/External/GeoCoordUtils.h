#ifndef EXTERNAL_GEOCOORDUTILS_H
#define EXTERNAL_GEOCOORDUTILS_H
#pragma once

#include <chrono/core/ChVector.h>

namespace External {
	
	class GeoCoordUtils {
	public:
		static chrono::Vector toGeodetic(const chrono::Vector& ecef_XYZ);
		static chrono::Vector toECEF(const chrono::Vector& geodetic_LLH);

	private:
		GeoCoordUtils() = default;
		~GeoCoordUtils() = default;

	};

}

#endif
