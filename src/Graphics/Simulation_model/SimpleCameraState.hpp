#ifndef GRAPHICS_SIMPLECAMERASTATE_HPP
#define GRAPHICS_SIMPLECAMERASTATE_HPP
#pragma once

#include <glm/vec3.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Graphics {
	
	struct SimpleCameraState {
		glm::dvec3 mPosition_highP;
		glm::dquat mOrientation_highP;
	};

}

#endif
