#ifndef EXTERNAL_GEOCOORDUTILS_H
#define EXTERNAL_GEOCOORDUTILS_H
#pragma once

#include "Earth.h"

#include <glm/vec3.hpp>

namespace External {
	
	class GeoCoordUtils {
	public:
		static glm::dvec3 toGeodetic(const glm::dvec3& ecef_XYZ);
		static glm::dvec3 toECEF(const glm::dvec3& geodetic_LLH);

	private:
		GeoCoordUtils() = default;
		~GeoCoordUtils() = default;

	};

}

#endif
