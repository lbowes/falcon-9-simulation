#ifndef F9SIM_GRAPHICS_STATESNAPSHOT_H_
#define F9SIM_GRAPHICS_STATESNAPSHOT_H_


#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>


namespace F9Sim {
namespace Graphics {


struct StateSnapshot {
    struct Cube {
        glm::dvec3 position;
        glm::dquat orientation;
    };

    Cube cube1;
    Cube cube2;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_STATESNAPSHOT_H_

