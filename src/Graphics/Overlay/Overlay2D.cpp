#include "Overlay2D.h"

namespace Graphics {

	Overlay2D::Overlay2D(Physics::Hardware::Falcon9& simDataSource, float& playbackSpeedHandle, float& timeHandle, float windowAspect) :
		mUILayer(simDataSource, playbackSpeedHandle, timeHandle),
		mVehicleOverlay(simDataSource, windowAspect)
	{ }

	void Overlay2D::render(glm::mat4 viewProjection, float windowAspect, glm::vec2 windowDimensions, float simulatedTime_s) {
		mUILayer.render(windowDimensions, simulatedTime_s);
		mVehicleOverlay.render(viewProjection, windowAspect, windowDimensions);
	}

}