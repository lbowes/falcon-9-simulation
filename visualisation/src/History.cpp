#include "History.h"

#include "../3rd_party/imgui/imgui.h"
#include <iostream>


namespace F9Sim {
namespace Graphics {


History::History(const nlohmann::json& data) :
    temp(data) {

    // todo: load and store whatever information is necessary to make `stateAt` behave correctly
    StateSnapshot timeStep = StateSnapshot(data[1]);
    printf("time_s: %f\n", timeStep.m_time_s);
}


StateSnapshot History::stateAt(double time) const {
    static int snapshotNum = 0;
    ImGui::Begin("history debug");
    ImGui::SliderInt("Snapshot No.", &snapshotNum, 0, 300);
    ImGui::End();
    StateSnapshot output(temp[snapshotNum]);
    return output;
}


} // namespace Graphics
} // namespace F9Sim
