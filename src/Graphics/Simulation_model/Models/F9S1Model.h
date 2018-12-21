//This class handles the rendered Falcon_9 model. The actual physical simulation of the rocket
//should not be concered in any way with rendering, Meshes, Models, Cameras etc. This class will
//serve as a visible skin for the vehicle being simulated.

#ifndef GRAPHICS_F9S1MODEL_H
#define GRAPHICS_F9S1MODEL_H
#pragma once

#include "Graphics/Simulation_model/ISimulationModel.hpp"
#include "Graphics/Simulation_model/Models/AllComponentMeshes.h"

#include <GraphicsFramework/Renderer.h>
#include <GraphicsFramework/Model3D.h>

namespace Physics {
	namespace Hardware {
		class Falcon9Stage1;
	}
}

namespace Graphics {

	class F9S1Model : public ISimulationModel {
	private:
		const Physics::Hardware::Falcon9Stage1& mDataSource;
		
		GF::Model3D mModel;
		
		GF::Graphics::Shader* mMainShader = nullptr;
		
		std::vector<std::unique_ptr<IStageComponentMesh>> mComponentMeshes;
		
		glm::dmat4 mTotalTransform_OGL;

	public:
		F9S1Model(const Physics::Hardware::Falcon9Stage1& dataSource, GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket);
		~F9S1Model() = default;

		glm::dmat4 getTotalTransform_OGL() const { return mTotalTransform_OGL; }

	private:
		void loadResources();
		void addComponentModels();

		virtual void updateAllTransforms_OGL(glm::dvec3 currentCameraPosition);
		virtual void makeRenderCalls();

	};

	

}

#endif