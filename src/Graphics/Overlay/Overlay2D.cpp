#include "Overlay2D.h"

namespace Graphics {

	Overlay2D::Overlay2D(Physics::Hardware::Falcon9& simDataSource, float& playbackSpeedHandle, float windowAspect) :
		mUILayer(simDataSource, playbackSpeedHandle)//,
		mVehicleOverlay(simDataSource, windowAspect)
	{ }

	void Overlay2D::render(glm::mat4 viewProjection, float windowAspect, glm::vec2 windowDimensions) {
		mUILayer.render(windowDimensions);
		mVehicleOverlay.render(viewProjection, windowAspect, windowDimensions);
	}

}