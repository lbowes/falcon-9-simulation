#ifndef GRAPHICS_PHYSICSOVERLAY_H
#define GRAPHICS_PHYSICSOVERLAY_H
#pragma once

#include "Marker2DOverlay.h"
//#include "CoordFrameOverlay.h"

#include <GraphicsFramework/Renderer.h>
#include <GraphicsFramework/ResourceSet.h>
#include <GraphicsFramework/OrthographicCamera.h>

namespace Physics {
	namespace Hardware {
		class Falcon9;
	}
}

namespace Graphics {

	class VehicleOverlay {
	private:
		Physics::Hardware::Falcon9& mDataSource;
		
		struct MarkedLocations {
			glm::vec3
				origin,
				centreMass,
				S1Origin,
				S1CentreMass,
				S2Origin,
				S2CentreMass;

			std::vector<glm::vec3>
				enginesOrigins = std::vector<glm::vec3>(9),
				enginesCoM = std::vector<glm::vec3>(9),
				legsOrigins = std::vector<glm::vec3>(4),
				legsCoM = std::vector<glm::vec3>(4),
				gridFinsOrigins = std::vector<glm::vec3>(4),
				gridFinsCoM = std::vector<glm::vec3>(4),
				tanksOrigins = std::vector<glm::vec3>(2),
				tanksCoM = std::vector<glm::vec3>(2);
		};
		MarkedLocations mMarkedLocations;
		
		//struct CoordFrameOrientations {
		//	glm::mat4
		//		LV,
		//		S1,
		//		S2;

		//	std::vector<glm::mat4>
		//		engines = std::vector<glm::mat4>(9),
		//		legs = std::vector<glm::mat4>(4),
		//		gridFins = std::vector<glm::mat4>(4),
		//		tanks = std::vector<glm::mat4>(2);
		//};
		//CoordFrameOrientations mCoordFrameOrientations;

		Marker2DOverlay mMarker2DOverlay;
		//CoordFrameOverlay mCoordFrame2DOverlay;

		GF::Graphics::Renderer mRenderer;
		GF::ResourceSet mResourceContainer;
		std::unique_ptr<GF::OrthographicCamera> mOrthoCam;

	public:
		VehicleOverlay(Physics::Hardware::Falcon9& simDataSource, float windowAspect);
		~VehicleOverlay() = default;

	public:
		void render(glm::mat4 viewProjection, float windowAspect, glm::vec2 windowDimensions);

	private:
		void load(float windowAspect);
		void addAllMarkers();
		void updateAllMarkers();
		//void addAllCoordFrames();
		//void updateAllCoordFrames();

	};

}

#endif
