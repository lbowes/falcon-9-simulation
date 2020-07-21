#ifndef F9SIM_GRAPHICS_ANIMATION_H_
#define F9SIM_GRAPHICS_ANIMATION_H_
#pragma once


#include "../3rd_party/json.hpp"
#include "StateSnapshot.h"


namespace F9Sim {
namespace Graphics {


class Animation {
private:
    double m_duration_s;
    double m_startTime_s;
    double m_endTime_s;
    unsigned short m_keyFramesPerSec;
    std::vector<StateSnapshot> m_snapshots;

public:
    Animation(const nlohmann::json& data, unsigned short keyFramesPerSec);
    ~Animation() = default;

    StateSnapshot stateAt(double time) const;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_ANIMATION_H_

