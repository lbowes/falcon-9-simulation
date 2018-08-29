#ifndef GRAPHICS_GASTHRUSTERMESH_H
#define GRAPHICS_GASTHRUSTERMESH_H
#pragma once

#include "IStageComponentMesh.hpp"

namespace Physics {
	namespace Hardware {
		class GasThruster;
	}
}

namespace Graphics {
	
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

		void loadResources();
		void updateResources(glm::mat4 stageModelTransform_OGL);

	};

}

#endif
