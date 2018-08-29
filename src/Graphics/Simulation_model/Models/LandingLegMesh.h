#ifndef LANDINGLEGMESH_H
#define LANDINGLEGMESH_H
#pragma once

#include <glm/mat4x4.hpp>
#include <vector>
#include <algorithm>

namespace Physics {
	namespace Hardware {
		class LandingLeg;
	}
}

namespace Graphics {
	class LandingLegMesh {
	private:
		Physics::Hardware::LandingLeg& mLandingLegData;

		//The first transform in this vector maps onto the first cylinder in the piston (at the stage mount end) etc.
		std::vector<glm::mat4> mPistonCylinderTransformMap;

		glm::mat4 mTransform_OGL;

	public:
		LandingLegMesh(Physics::Hardware::LandingLeg& landingLegData);
		~LandingLegMesh() = default;

		void updateTransform_OGL(glm::mat4 stageModelTransform_OGL);
		glm::mat4 getCylinderTransform(unsigned char cylinderNumber);

		inline glm::mat4 getTransform_OGL() const { return mTransform_OGL; }

	private:
		void updateCylinderTransforms_OGL(float pistonLength, float angleDownFromVertical, glm::mat4 stageModelTransform_OGL);

	};
}

#endif
