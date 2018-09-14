//This class handles the rendered Falcon_9 model. The actual physical simulation of the rocket
//should not be concered in any way with rendering, Meshes, Models, Cameras etc. This class will
//serve as a visible skin for the vehicle being simulated.

#ifndef GRAPHICS_F9S1MODEL_H
#define GRAPHICS_F9S1MODEL_H
#pragma once

#include "EngineMesh.h"
#include "GasThrusterMesh.h"
#include "Graphics/Simulation_model/ISimulationModel.hpp"
#include "Physics/DynamicSimState.h"

#include <GraphicsFramework/Renderer.h>
#include <GraphicsFramework/Model3D.h>

#define BOX_MODELS 0

namespace Graphics {

	class F9S1Model : public ISimulationModel {
	private:
		GF::Model3D mModel;
		GF::Graphics::Shader* mMainShader = nullptr;
		std::vector<std::unique_ptr<IStageComponentMesh>> mComponentMeshes;
		//const Physics::Hardware::Falcon9Stage1& mStage1Data;
		glm::dmat4 mTotalTransform_OGL;

	public:
		F9S1Model(const Physics::Hardware::Falcon9Stage1& stage1Data, GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket);
		~F9S1Model() = default;

		glm::dmat4 getTotalTransform_OGL() const { return mTotalTransform_OGL; }

	private:
		void loadResources();
		void addComponentModels();

		virtual void updateAllTransforms_OGL(const Physics::DynamicSimState::Falcon9::Stage1& stage1, glm::dvec3 currentCameraPosition);
		virtual void makeRenderCalls();

	};

	class LandingLegMesh : public IStageComponentMesh {
	private:
		static unsigned char mNumInstances;
		const Physics::Hardware::LandingLeg& mDataSource;
		GF::Graphics::Mesh* mAFrameMesh = nullptr;
		std::vector<GF::Graphics::Mesh*> mPistonCylinderMeshes;

	public:
		LandingLegMesh(const Physics::Hardware::LandingLeg& dataSource, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel);
		~LandingLegMesh() = default;
	
	private:
		virtual void loadResources();
		virtual void updateResources(glm::mat4 stageModelTransform_OGL);
	
	};

	class GridFinMesh : public IStageComponentMesh {
	private:
		static unsigned char mNumInstances;
		const Physics::Hardware::GridFin& mDataSource;
		
		GF::Graphics::Mesh
			*mHingeMesh = nullptr,
			*mFinMesh = nullptr;

	public:
		GridFinMesh(const Physics::Hardware::GridFin& dataSource, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel);
		~GridFinMesh() = default;

	private:
		virtual void loadResources();
		virtual void updateResources(glm::mat4 stageModelTransform_OGL);
	};

}

#endif