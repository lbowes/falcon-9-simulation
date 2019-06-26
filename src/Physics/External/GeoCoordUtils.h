#ifndef EXTERNAL_GEOCOORDUTILS_H
#define EXTERNAL_GEOCOORDUTILS_H
#pragma once

#include <chrono/core/ChVector.h>

namespace External {
	
	class GeoCoordUtils {
	public:
		static chrono::ChVector<> toGeodetic(const chrono::Vector& ecef_XYZ);
		static chrono::ChVector<> toECEF(const chrono::Vector& geodetic_LLH);

	private:
		GeoCoordUtils() = default;
		~GeoCoordUtils() = default;

	};

}

#endif // EXTERNAL_GEOCOORDUTILS_H
