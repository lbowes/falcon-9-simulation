#ifndef GRAPHICS_OVERLAY2D_H
#define GRAPHICS_OVERLAY2D_H
#pragma once

#include "UILayer.h"
#include "VehicleOverlay.h"

namespace Graphics {

	class Overlay2D {
	private:
		UILayer mUILayer;
		VehicleOverlay mVehicleOverlay;

	public:
		Overlay2D(float& playbackSpeedHandle, float windowAspect);
		~Overlay2D() = default;
	
		void render(const Physics::DynamicSimState& currentState, glm::mat4 viewProjection, float windowAspect, glm::vec2 windowDimensions);

	};

}

#endif
