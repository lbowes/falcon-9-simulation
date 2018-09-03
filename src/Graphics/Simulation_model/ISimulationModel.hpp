#ifndef GRAPHICS_ISIMULATIONMODEL_H
#define GRAPHICS_ISIMULATIONMODEL_H
#pragma once

#include <GraphicsFramework/Renderer.h>
#include <GraphicsFramework/ResourceSet.h>

namespace Graphics {
	
	class ISimulationModel {
	protected:
		GF::Graphics::Renderer& mRenderer;
		GF::ResourceSet& mResourceBucket;

	public:
		ISimulationModel(GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket) :
			mRenderer(renderer),
			mResourceBucket(resourceBucket)
		{ }
		
		virtual ~ISimulationModel() = default;

		//This combines the two pure virtual functions below. It can be called from outside the class.
		void render(glm::dvec3 currentCameraPosition) {
			updateAllTransforms_OGL(currentCameraPosition);
			makeRenderCalls();
		}

	protected:
		//Use our position and orientation (found behind ISimpleCameraState) and the current Camera's state
		//to update all the transforms belonging to this model.
		virtual void updateAllTransforms_OGL(glm::dvec3 currentCameraPosition) = 0;

		//All models will implement this function. It sends the render commands of Models or Meshes that it owns.
		virtual void makeRenderCalls() = 0;

	};

}

#endif
