#ifndef F9SIM_GRAPHICS_STATESNAPSHOT_H_
#define F9SIM_GRAPHICS_STATESNAPSHOT_H_


#include "../3rd_party/json.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>


namespace F9Sim {
namespace Graphics {


class StateSnapshot {
    //temp
public:
    //private:
    struct Cube {
        glm::dvec3 position;
        glm::dquat orientation;
    };

    Cube cube1;
    Cube cube2;

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

