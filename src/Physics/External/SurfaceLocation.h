#ifndef PHYSICS_EXTERNAL_SURFACELOCATION_H
#define PHYSICS_EXTERNAL_SURFACELOCATION_H
#pragma once

#include "GeoCoordUtils.h"

#include <PhysicsFramework/CoordTransform3D.h>
#include <string>

namespace Physics {
	namespace External {

		class SurfaceLocation {
		private:
			std::string mName;
			glm::dvec3 mGPSCoordinate_LLH;
			CoordTransform3D mEUN_to_ECEF;

		public:
			SurfaceLocation(const std::string& name = "", const glm::dvec3& gpsCoord_LLH = { 0.0, 0.0, 0.0 });
			void operator=(const SurfaceLocation& other);
			~SurfaceLocation() = default;

			void setName(const std::string& name) { mName = name; }
			void setGPSPosition(const glm::dvec3& gpsCoord_LLH);

			const CoordTransform3D& getEUN_to_ECEFTransform() const { return mEUN_to_ECEF; }

		private:
			void updateENU_to_ECEFTransform();

		};

	}
}

#endif