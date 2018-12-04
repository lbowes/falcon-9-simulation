#include "UILayer.h"
#include "Physics/Hardware/Falcon_9/Falcon9.h"

#include <sstream>
#include <iomanip>

namespace Graphics {
	
	UILayer::UILayer(const Physics::Hardware::Falcon9& simDataSource, float& playbackSpeedHandle, float& timeHandle) :
		mDataSource(simDataSource),
		mPlaybackSpeedHandle(playbackSpeedHandle),
		mTimeHandle(timeHandle)
	{
		load();
		
		//temp testing graphs
		stage1_testGraph.setBackgroundColour({ 1.0f, 1.0f, 1.0f, 0.06f });
		stage1_testGraph.setDataColour({1.0f, 1.0f, 1.0f, 0.3f});
		stage1_testGraph.setLowerLimit({ -10'000.0f, 0.0f });
		stage1_testGraph.setUpperLimit({ 0.0f, 10'000.0f });
		
		stage2_testGraph.setBackgroundColour({ 1.0f, 1.0f, 1.0f, 0.06f });
		stage2_testGraph.setDataColour({ 1.0f, 1.0f, 1.0f, 0.3f });
		stage2_testGraph.setLowerLimit({ -10'000.0f, 0.0f });
		stage2_testGraph.setUpperLimit({ 0.0f, 10'000.0f });

		stage2_testGraph.addDataPoint({0.0f, 10.0f});
		stage2_testGraph.addDataPoint({0.0f, 0.0f});
		//
	}

	void UILayer::render(glm::vec2 mainWindowSize, float simulatedTime_s) {
		mTextDrawList = ImGui::GetOverlayDrawList();

		stage1PhysicsState();
		simulationControls(simulatedTime_s);

		const glm::dvec3 s1Position = mDataSource.getStage1().getState().getCoMPosition_world();
		static double lastTime = 0.0;
		if (s1Position.y > 30.0) {
			double
				currentTime = glfwGetTime(),
				delta = currentTime - lastTime;

			if (delta > 0.5) {
				stage1_testGraph.addDataPoint({ static_cast<float>(s1Position.z), static_cast<float>(s1Position.y) });
				stage2_testGraph.addDataPoint({ static_cast<float>(s1Position.z), static_cast<float>(s1Position.y) });

				delta = 0.0;
				lastTime = currentTime;
			}
		}

		ImGui::SetNextWindowSize(ImVec2(800, 800 + 22));
		ImGui::Begin("Flight profile", NULL, ImGuiWindowFlags_NoResize);
		
		stage1_testGraph.render("Flight profile", mainWindowSize);
		stage2_testGraph.render("Flight profile", mainWindowSize);
		
		ImGui::End();
		//
	}

	void UILayer::load() const {
		//Load the ImGui style
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 4.0f);

		ImVec4 barColourMain = ImVec4(0.0f, 0.2588235294117647f, 0.4352941176470588f, 1.0f);

		ImGui::PushStyleColor(ImGuiCol_TitleBg,          barColourMain);
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive,    barColourMain);
		ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, barColourMain);

		ImGui::PushStyleColor(ImGuiCol_WindowBg,         ImVec4(0.17647058823f, 0.17647058823f, 0.18823529411f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_Border,           ImVec4(0.64705882352f, 0.64705882352f, 0.64705882352f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ChildWindowBg,    ImVec4(0.11764705882f, 0.11764705882f, 0.11764705882f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg,        barColourMain);
	}

	void UILayer::stage1PhysicsState() const {
		using namespace std;

		const Physics::Hardware::Falcon9Stage1& s1 = mDataSource.getStage1();
		const State& s1State = s1.getState();

		string dataString = "";

		glm::dvec3 vec = s1State.getCoMPosition_world();
		dataString += "Position:            " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";

		vec = s1State.getVelocity_world();
		dataString += "Velocity:            " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";

		vec = s1.getAccel_world();
		dataString += "Linear acceleration: " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";
		
		vec = s1State.getMomentum_world();
		dataString += "Momentum:            " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";

		vec = s1State.getAngularVelocity_world();
		dataString += "Angular velocity:    " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";

		vec = s1State.getAngularMomentum_world();
		dataString += "Angular momentum:    " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";

		dataString += "Total mass:          " + to_string(s1State.getMass_local().getValue());

		mTextDrawList->AddText(ImVec2(0, 0), ImColor(1.0f, 1.0f, 1.0f), dataString.c_str());
	}

	void UILayer::simulationControls(float simulatedTime_s) const {
		using namespace Physics;
		using namespace ImGui;

		//temp
		Begin("Simulation controls", 0); //ImGuiWindowFlags_AlwaysAutoResize
		SliderFloat("Speed", &mPlaybackSpeedHandle, 0.0f, 10.0f);
		
		if (Button("Pause")) 
			mPlaybackSpeedHandle = 0.0f; 
		
		SameLine();
		if (Button("Resume")) 
			mPlaybackSpeedHandle = 1.0f; 
		
		SameLine();
		if (Button("50%%")) 
			mPlaybackSpeedHandle = 0.5f;

		SliderFloat("Time", &mTimeHandle, 0.0f, simulatedTime_s);

		End();
		//
	}

}
