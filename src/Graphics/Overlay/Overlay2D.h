#ifndef GRAPHICS_OVERLAY2D_H
#define GRAPHICS_OVERLAY2D_H
#pragma once

#include "UILayer.h"
#include "VehicleOverlay.h"

namespace Physics {
	namespace Hardware {
		class Falcon9;
	}
}

namespace Graphics {

	class Overlay2D {
	private:
		UILayer mUILayer;
		VehicleOverlay mVehicleOverlay;

	public:
		Overlay2D(Physics::Hardware::Falcon9& simDataSource, float& simulationSpeedHandle, float windowAspect);
		~Overlay2D() = default;
	
		void render(glm::mat4 viewProjection, float windowAspect, glm::vec2 windowDimensions);

	};

}

#endif
