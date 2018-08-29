#ifndef GRAPHICS_F9S2MODEL_H
#define GRAPHICS_F9S2MODEL_H
#pragma once

#include "EngineMesh.h"
#include "GasThrusterMesh.h"
#include "Graphics/Simulation_model/ISimulationModel.hpp"

#include <GraphicsFramework/Renderer.h>
#include <GraphicsFramework/Model3D.h>

namespace Physics {
	namespace Hardware {
		class Falcon9Stage2;
	}
}

namespace Graphics {
	
	class F9S2Model : public ISimulationModel {
	private:
		GF::Model3D mModel;
		std::vector<std::unique_ptr<IStageComponentMesh>> mComponentModels;
		Physics::Hardware::Falcon9Stage2& mStage2Data;

	public:
		F9S2Model(Physics::Hardware::Falcon9Stage2& stage2Data, GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket);
		~F9S2Model() = default;

	private:
		void loadResources();
		void addComponentModels();
		void update(glm::mat4 totalStageTransform_OGL);

		virtual void updateAllTransforms_OGL(SimpleCameraState currentCameraState);
		virtual void makeRenderCalls();

	};

}

#endif
