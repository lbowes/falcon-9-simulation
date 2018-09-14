#include "Overlay2D.h"

namespace Graphics {

	Overlay2D::Overlay2D(float& playbackSpeedHandle, float windowAspect) :
		mUILayer(playbackSpeedHandle),
		mVehicleOverlay(windowAspect)
	{ }

	void Overlay2D::render(const Physics::DynamicSimState& currentState, glm::mat4 viewProjection, float windowAspect, glm::vec2 windowDimensions) {
		mUILayer.render(currentState.falcon9.S1, windowDimensions);
		mVehicleOverlay.render(currentState.falcon9, viewProjection, windowAspect, windowDimensions);
	}

}