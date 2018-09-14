#ifndef GRAPHICS_LANDINGLEGMESH_H
#define GRAPHICS_LANDINGLEGMESH_H
#pragma once

#include "Physics/StaticSimState.h"
#include "Physics/DynamicSimState.h"

#include <glm/mat4x4.hpp>
#include <vector>
#include <algorithm>

namespace Graphics {
	
	class LandingLegMesh {
	private:
		const Physics::DynamicSimState::Falcon9::Stage1::LandingLeg& mDataSource;
		
		glm::mat4 mTransform_OGL;

		//The first transform in this vector maps onto the first cylinder in the piston (at the stage mount end) etc.
		std::vector<glm::mat4> mPistonCylinderTransformMap;

	public:
		LandingLegMesh(const Physics::StaticSimState::Falcon9::Stage1::LandingLegs& legs, const Physics::DynamicSimState::Falcon9::Stage1::LandingLeg& dataSource);
		~LandingLegMesh() = default;

		void updateTransform_OGL(glm::mat4 stageModelTransform_OGL);
		glm::mat4 getCylinderTransform(unsigned char cylinderNumber);

		glm::mat4 getTransform_OGL() const { return mTransform_OGL; }

	private:
		void updateCylinderTransforms_OGL(float pistonLength, float angleDownFromVertical, glm::mat4 stageModelTransform_OGL);

	};

}

#endif
