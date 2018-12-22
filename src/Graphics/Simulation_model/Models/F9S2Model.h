#ifndef GRAPHICS_F9S2MODEL_H
#define GRAPHICS_F9S2MODEL_H
#pragma once

#include "Graphics/Simulation_model/ISimulationModel.hpp"
#include "Graphics/Simulation_model/Models/AllComponentMeshes.h"

#include <GraphicsFramework/Renderer.h>
#include <GraphicsFramework/Model3D.h>

namespace Physics {
	namespace Hardware {
		class Falcon9Stage2Composite;
	}
}

namespace Graphics {
	
	class F9S2Model : public ISimulationModel {
	private:
		GF::Model3D mModel;
		
		std::unique_ptr<IStageComponentMesh> mEngineMesh;
		
		std::pair<std::unique_ptr<FairingHalfMesh>, std::unique_ptr<FairingHalfMesh>> mFairingHalfMeshes;

		const Physics::Hardware::Falcon9Stage2Composite& mDataSource;

	public:
		F9S2Model(const Physics::Hardware::Falcon9Stage2Composite& dataSource, GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket);
		~F9S2Model() = default;

	private:
		void loadResources();
		void addComponentModels();

		virtual void updateAllTransforms_OGL(glm::dvec3 currentCameraPos_world);
		virtual void makeRenderCalls();

	};

}

#endif
