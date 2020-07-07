#include "StateSnapshot.h"

#include <glm/glm.hpp>


namespace F9Sim {
namespace Graphics {


StateSnapshot::StateSnapshot() :
    cube1({.position = glm::dvec3(), .orientation = glm::dvec3()}),
    cube2({.position = glm::dvec3(), .orientation = glm::dvec3()}) {
}


StateSnapshot::StateSnapshot(const nlohmann::json& data) {
    for(int i = 0; i < 3; i++)
        data["position_world"][i].get_to(cube1.position[i]);

    for(int i = 0; i < 4; i++)
        data["orientation_world"][i].get_to(cube1.orientation[i]);
}


StateSnapshot StateSnapshot::lerp(const StateSnapshot& a, const StateSnapshot& b, double x) {
    StateSnapshot output;

    output.cube1 = lerp(a.cube1, b.cube1, x);
    output.cube2 = lerp(a.cube2, b.cube2, x);

    return output;
}


StateSnapshot::Cube StateSnapshot::lerp(const Cube& a, const Cube& b, double x) {
    Cube output;

    output.position = glm::mix(a.position, b.position, x);
    output.orientation = glm::slerp(a.orientation, b.orientation, x);

    return output;
}


} // namespace Graphics
} // namespace F9Sim
