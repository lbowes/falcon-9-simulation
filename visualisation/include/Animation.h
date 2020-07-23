#ifndef F9SIM_GRAPHICS_ANIMATION_H_
#define F9SIM_GRAPHICS_ANIMATION_H_
#pragma once


#include "../3rd_party/json.hpp"
#include "StateSnapshot.h"


namespace F9Sim {
namespace Graphics {


class Animation {
private:
    float m_duration_s;
    float m_snapshotInterval_s;
    std::vector<StateSnapshot> m_snapshots;

public:
    Animation(const nlohmann::json& data, float snapshotInterval_s);
    ~Animation() = default;

    StateSnapshot stateAt(float time_s) const;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_ANIMATION_H_

