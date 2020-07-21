#include "Animation.h"
#include "../3rd_party/imgui/imgui.h"

#include <algorithm>
#include <iostream>


namespace F9Sim {
namespace Graphics {


Animation::Animation(const nlohmann::json& data, unsigned short keyFramesPerSec) :
    m_startTime_s(0.0),
    m_endTime_s(0.0),
    m_duration_s(0.0),
    m_keyFramesPerSec(keyFramesPerSec) {

    // Load the timestamps of each snapshot in the simulation data
    std::vector<float> snapshotTimes;

    for(auto& state : data)
        snapshotTimes.push_back(state["time_s"]);

    if(!snapshotTimes.empty()) {
        m_startTime_s = snapshotTimes.front();
        m_endTime_s = snapshotTimes.back();
        m_duration_s = m_endTime_s - m_startTime_s;
    }

    // todo: load and store whatever information is necessary to make `stateAt` behave correctly
}


StateSnapshot Animation::stateAt(double time) const {
    time = std::clamp(time, m_startTime_s, m_endTime_s);

    StateSnapshot output;

    if(!m_snapshots.empty()) {
        // todo
    }

    return output;
}


} // namespace Graphics
} // namespace F9Sim
