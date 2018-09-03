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
		const double mHeight = 3.0;
		const glm::dvec3 mPosition = { 0.0, -mHeight, 0.0 };		

	public:
		ASDSModel(GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket, GF::Graphics::Shader& shader);
		~ASDSModel() = default;

	private:
		void loadResources();

		virtual void updateAllTransforms_OGL(glm::dvec3 currentCameraPosition);
		virtual void makeRenderCalls();

	};

}

#endif