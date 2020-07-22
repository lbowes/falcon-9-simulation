#include "StateSnapshot.h"

#include <glm/glm.hpp>


namespace F9Sim {
namespace Graphics {


StateSnapshot::StateSnapshot() :
    m_cube1({.position = glm::dvec3(), .orientation = glm::dquat()}),
    m_cube2({.position = glm::dvec3(), .orientation = glm::dquat()}),
    m_time_s(0.0) {
}


StateSnapshot::StateSnapshot(const nlohmann::json& data) {
    // Time
    data["time_s"].get_to(m_time_s);

    // Cube 1
    const nlohmann::json& cube1 = data["cube"]["cube1"];
    for(int i = 0; i < 3; i++)
        cube1["position_world"][i].get_to(m_cube1.position[i]);

    for(int i = 0; i < 4; i++)
        cube1["orientation_world"][i].get_to(m_cube1.orientation[i]);

    // Cube 2
    const nlohmann::json& cube2 = data["cube"]["cube2"];
    for(int i = 0; i < 3; i++)
        cube2["position_world"][i].get_to(m_cube2.position[i]);

    for(int i = 0; i < 4; i++)
        cube2["orientation_world"][i].get_to(m_cube2.orientation[i]);

    //m_cube2.position = glm::dvec3();
    //m_cube2.orientation = glm::dquat();
}


StateSnapshot StateSnapshot::lerp(const StateSnapshot& a, const StateSnapshot& b, double x) {
    StateSnapshot output;

    output.m_cube1 = lerp(a.m_cube1, b.m_cube1, x);
    output.m_cube2 = lerp(a.m_cube2, b.m_cube2, x);

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
