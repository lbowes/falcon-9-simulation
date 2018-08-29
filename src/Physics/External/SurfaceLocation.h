#ifndef PHYSICS_EXTERNAL_SURFACELOCATION_H
#define PHYSICS_EXTERNAL_SURFACELOCATION_H
#pragma once

#include "GeoCoordUtils.h"

#include <GraphicsFramework/CoordTransform3D.h>
#include <string>

namespace Physics {
	namespace External {

		class SurfaceLocation {
		private:
			glm::dvec3 mGPSCoordinate_LLH;
			GF::CoordTransform3D mEUNtoECEF;
			const std::string& mName;

		public:
			SurfaceLocation(const glm::dvec3& gpsCoord_LLH, const std::string& name = "");
			~SurfaceLocation() = default;

			const GF::CoordTransform3D& getEUNtoECEFCoordTransform() const { return mEUNtoECEF; }

		};

	}
}

#endif