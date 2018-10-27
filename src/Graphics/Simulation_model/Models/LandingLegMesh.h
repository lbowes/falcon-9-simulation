#ifndef GRAPHICS_LANDINGLEGMESH_H
#define GRAPHICS_LANDINGLEGMESH_H
#pragma once

#include "Graphics/Simulation_model/Models/IStageComponentMesh.hpp"
#include "definitions.h"

#include <glm/mat4x4.hpp>
#include <vector>
#include <algorithm>

namespace Physics {
	namespace Hardware {
		class LandingLeg;
	}
}

namespace Graphics {
	
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

}

#endif
