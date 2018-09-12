#include "Overlay2D.h"

namespace Graphics {

	Overlay2D::Overlay2D(float& playbackSpeedHandle, float windowAspect) :
		mUILayer(playbackSpeedHandle),
		mVehicleOverlay(windowAspect)
	{ }

	void Overlay2D::render(const Physics::SimState::Falcon9& falcon9, glm::mat4 viewProjection, float windowAspect, glm::vec2 windowDimensions) {
		mUILayer.render(falcon9.S1, windowDimensions);
		mVehicleOverlay.render(falcon9, viewProjection, windowAspect, windowDimensions);
	}

}