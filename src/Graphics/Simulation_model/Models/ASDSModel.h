#ifndef GRAPHICS_ASDSMODEL_H
#define GRAPHICS_ASDSMODEL_H
#pragma once

#include "Graphics/Simulation_model/ISimulationModel.hpp"

#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <GraphicsFramework/ResourceSet.h>

namespace Graphics {
	
	class ASDSModel : public ISimulationModel {
	private:
		GF::Graphics::Shader& mShader;
		GF::Graphics::Mesh* mMeshHandle = nullptr;
		
		float mHeight = 0.0f;

	public:
		ASDSModel(GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket, GF::Graphics::Shader& shader, float height);
		~ASDSModel() = default;

	private:
		void loadResources();

		virtual void updateAllTransforms_OGL(SimpleCameraState currentCameraState);
		virtual void makeRenderCalls();

	};

}

#endif