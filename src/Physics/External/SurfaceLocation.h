#ifndef PHYSICS_EXTERNAL_SURFACELOCATION_H
#define PHYSICS_EXTERNAL_SURFACELOCATION_H
#pragma once

#include <chrono/core/ChFrame.h>
#include <string>

namespace Physics {
	namespace External {

		class SurfaceLocation {
		private:
			std::string mName;
			chrono::ChVector<> mGPSCoordinate_LLH;
			chrono::ChFrame<> mEUN_to_ECEF;

		public:
			SurfaceLocation(const std::string& name = "", const chrono::ChVector<>& gpsCoord_LLH = {0, 0, 0});
			void operator=(const SurfaceLocation& other);
			~SurfaceLocation() = default;

			void setName(const std::string& name) { mName = name; }
			void setGPSPosition(const chrono::ChVector<>& gpsCoord_LLH);

			const chrono::ChFrame<>& getEUN_to_ECEFTransform() const { return mEUN_to_ECEF; }

		private:
			void updateENU_to_ECEFTransform();

		};

	}
}

#endif // PHYSICS_EXTERNAL_SURFACELOCATION_H