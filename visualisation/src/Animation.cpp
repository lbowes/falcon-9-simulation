#include "Animation.h"
#include "../3rd_party/imgui/imgui.h"

#include <algorithm>
#include <iostream>


namespace F9Sim {
namespace Graphics {


Animation::Animation(const nlohmann::json& data, float snapshotInterval_s) :
    m_duration_s(0.0),
    m_snapshotInterval_s(snapshotInterval_s) {

    // This initialisation assumes that:
    // 1. the first snapshot is at time 0
    // 2. the snapshots are spaced equally

    for(auto& state : data)
        m_snapshots.push_back(state);
}


StateSnapshot Animation::stateAt(float time_s) const {
    static float time = 0.0f;
    ImGui::Begin("time control");
    ImGui::SliderFloat("time", &time, 0.0f, 10.0f);
    time = std::clamp(time, 0.0f, m_duration_s);

    StateSnapshot output;

    if(!m_snapshots.empty()) {
        const float s = floor(time / m_snapshotInterval_s);
        const float between = fmod(time, m_snapshotInterval_s) / m_snapshotInterval_s;

        const unsigned int lastSnapshotIdx = m_snapshots.size() - 1;
        const unsigned int previousSnapshotIdx = std::clamp(static_cast<unsigned int>(s), 0U, lastSnapshotIdx);

        const StateSnapshot previous = m_snapshots.at(previousSnapshotIdx);
        const StateSnapshot next = m_snapshots.at(std::clamp(previousSnapshotIdx + 1, 0U, lastSnapshotIdx));

        output = StateSnapshot::lerp(previous, next, between);
    }

    ImGui::End();
    return output;
}


} // namespace Graphics
} // namespace F9Sim
