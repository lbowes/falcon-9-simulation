#ifndef GRAPHICS_ENGINEMESH_H
#define GRAPHICS_ENGINEMESH_H
#pragma once

#include "IStageComponentMesh.hpp"

namespace Physics {
	namespace Hardware {
		class Engine;
	}
}

namespace Graphics {
	
	class EngineMesh : public IStageComponentMesh {
		friend class ExhaustJet;	
	private:
		static unsigned char mNumInstances;
		
		const std::string
			mName = "",
			mOBJFilePath = "";

		const Physics::Hardware::Engine& mEngineDataSource;

		GF::Graphics::Mesh* mMesh = nullptr;

	public:
		EngineMesh(const std::string& name, const std::string& objFilePath, const Physics::Hardware::Engine& engine, GF::ResourceSet& resourceBucket, GF::Model3D& parentModel);
		~EngineMesh() = default;

		void updateResources(glm::mat4 totalStageTransform_OGL);

	private:
		void loadResources();

	};

}

#endif