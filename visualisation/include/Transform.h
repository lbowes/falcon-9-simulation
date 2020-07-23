#ifndef F9SIM_GRAPHICS_TRANSFORM_H_
#define F9SIM_GRAPHICS_TRANSFORM_H_
#pragma once


#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>


struct Transform {
    glm::dvec3 position;
    glm::dquat orientation;
};


#endif // F9SIM_GRAPHICS_TRANSFORM_H_

