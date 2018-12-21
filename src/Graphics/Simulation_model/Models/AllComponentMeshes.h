//Stores all graphical component meshes for the simulation

#ifndef GRAPHICS_ALLCOMPONENTMESHES_H
#define GRAPHICS_ALLCOMPONENTMESHES_H
#pragma once

#include "IStageComponentMesh.hpp"
#include "definitions.h"

namespace Physics {
	namespace Hardware {
		class Engine;
		class GasThruster;
		class GridFin;
		class LandingLeg;
		class FairingHalf;
	}
}

namespace Graphics {

	//-------------------------------------------------------------------ENGINE-------------------------------------------------------------------

	class EngineMesh : public IStageComponentMesh {
		friend class ExhaustJet;	
	private:
		const std::string
			mName = "",
			mOBJFilePath = "";

		const Physics::Hardware::Engine& mEngineDataSource;

		GF::Graphics::Mesh* mMesh = nullptr;

	public:
		EngineMesh(const std::string& name, const std::string& objFilePath, const Physics::Hardware::Engine& engine, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel);
		~EngineMesh() = default;

	private:
		virtual void loadResources();
		virtual void updateResources(glm::mat4 totalStageTransform_OGL);

	};

	//-------------------------------------------------------------------GAS THRUSTER-------------------------------------------------------------------

	class GasThrusterMesh : public IStageComponentMesh {
	private:
		static constexpr float
			mPlumeLength = 2.1f,         //m
			mStartPlumeRadius = 0.0225f, //m
			mEndPlumeRadius = 0.34f;     //m
		
		static unsigned char mNumInstances;

		Physics::Hardware::GasThruster& mDataSource;

		GF::Graphics::Mesh* mGasMesh = nullptr;

	public:
		GasThrusterMesh(Physics::Hardware::GasThruster& dataSource, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel);
		~GasThrusterMesh() = default;

	private:
		virtual void loadResources();
		virtual void updateResources(glm::mat4 stageModelTransform_OGL);

	};

	//-------------------------------------------------------------------GRID FIN-------------------------------------------------------------------

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

	//-------------------------------------------------------------------LANDING LEG-------------------------------------------------------------------

	class LandingLegMesh : public IStageComponentMesh {
	private:
		static unsigned char mNumInstances;
		
		const Physics::Hardware::LandingLeg& mDataSource;

		GF::Graphics::Mesh
			*mAFrameMesh = nullptr,
			*mPusherMesh = nullptr;
		
		std::vector<GF::Graphics::Mesh*> mPistonCylinderMeshes;

	public:
		LandingLegMesh(const Physics::Hardware::LandingLeg& dataSource, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel);
		~LandingLegMesh() = default;
	
	private:
		virtual void loadResources();
		virtual void updateResources(glm::mat4 stageModelTransform_OGL);
	
	};

	//-------------------------------------------------------------------FAIRING HALF-------------------------------------------------------------------

	class FairingHalfMesh {
	private:
		static unsigned char mNumInstances;
		
		const Physics::Hardware::FairingHalf& mDataSource;

		GF::Graphics::Mesh* mMesh = nullptr;

	public:
		FairingHalfMesh(const Physics::Hardware::FairingHalf& dataSource, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel);
		~FairingHalfMesh() = default;
		
		void updateTransform_OGL(glm::dvec3 currentCameraPos_world);
	
	};


}

#endif
