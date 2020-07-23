#ifndef F9SIM_GRAPHICS_STATESNAPSHOT_H_
#define F9SIM_GRAPHICS_STATESNAPSHOT_H_


#include "../3rd_party/json.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>


namespace F9Sim {
namespace Graphics {


class StateSnapshot {
public:
    struct Cube {
        glm::dvec3 position;
        glm::dquat orientation;
    };

    Cube m_cube1;
    Cube m_cube2;

    double m_time_s;

public:
    StateSnapshot();
    StateSnapshot(const nlohmann::json& data);
    ~StateSnapshot() = default;

    static StateSnapshot lerp(const StateSnapshot& a, const StateSnapshot& b, double x);

private:
    static Cube lerp(const Cube& a, const Cube& b, double x);
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_STATESNAPSHOT_H_

