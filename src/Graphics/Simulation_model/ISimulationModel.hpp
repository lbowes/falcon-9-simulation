#ifndef GRAPHICS_ISIMULATIONMODEL_H
#define GRAPHICS_ISIMULATIONMODEL_H
#pragma once

#include "SimpleCameraState.hpp"

#include <GraphicsFramework/Renderer.h>
#include <GraphicsFramework/ResourceSet.h>

namespace Graphics {
	
	class ISimulationModel {
	protected:
		GF::Graphics::Renderer& mRenderer;
		GF::ResourceSet& mResourceBucket;

		SimpleCameraState mSimpleCameraState;

	public:
		ISimulationModel(GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket) :
			mRenderer(renderer),
			mResourceBucket(resourceBucket)
		{ }
		
		virtual ~ISimulationModel() = default;

		//This combines the two pure virtual functions below. It can be called from outside the class.
		void render(SimpleCameraState currentCameraState) {
			updateAllTransforms_OGL(currentCameraState);
			makeRenderCalls();
		}

	protected:
		//Use our position and orientation (found behind ISimpleCameraState) and the current Camera's state
		//to update all the transforms belonging to this model.
		virtual void updateAllTransforms_OGL(SimpleCameraState currentCameraState) = 0;

		//All models will implement this function. It sends the render commands of Models or Meshes that it owns.
		virtual void makeRenderCalls() = 0;

	};

}

#endif
