#include "UILayer.h"
#include "Physics/Hardware/Falcon_9/Falcon9.h"

#include <sstream>
#include <iomanip>

namespace Graphics {
	
	UILayer::UILayer(float& playbackSpeedHandle) :
		mSimSpeedHandle(playbackSpeedHandle)
	{
		load();
		
		//temp testing graphs
		stage1_testGraph.setBackgroundColour({ 0.0f, 0.0f, 0.0f, 0.06f });
		stage1_testGraph.setDataColour({1.0f, 1.0f, 1.0f, 0.3f});
		stage1_testGraph.setLowerLimit({ -900.0f, 0.0f });
		stage1_testGraph.setUpperLimit({ 150'000.0f, 150'000.0f });
		
		stage2_testGraph.setBackgroundColour({ 0.0f, 0.0f, 0.0f, 0.06f });
		stage2_testGraph.setDataColour({ 1.0f, 1.0f, 1.0f, 0.3f });
		stage2_testGraph.setLowerLimit({ -900.0f, 0.0f });
		stage2_testGraph.setUpperLimit({ 150'000.0f, 150'000.0f });
		//
	}

	void UILayer::render(const Physics::SimState::Falcon9::Stage1& stage1, glm::vec2 mainWindowSize) {
		mTextDrawList = ImGui::GetOverlayDrawList();

		stage1PhysicsState(stage1);
		simulationControls();

		static double lastTime = 0.0;
		if (stage1.mPosition.y > 0.0) {
			double
				currentTime = glfwGetTime(),
				delta = currentTime - lastTime;

			if (delta > 0.5) {
				stage1_testGraph.addDataPoint({ static_cast<float>(stage1.mPosition.z), static_cast<float>(stage1.mPosition.y) });
				stage2_testGraph.addDataPoint({ static_cast<float>(stage1.mPosition.z), static_cast<float>(stage1.mPosition.y) });

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

	void UILayer::stage1PhysicsState(const Physics::SimState::Falcon9::Stage1& stage1) const {
		using namespace std;

		string dataString = "";
		
		glm::dvec3 vec = stage1.mPosition;
		dataString += "Position:            " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";

		vec = stage1.mVelocity;
		dataString += "Velocity:            " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";

		vec = stage1.mAcceleration;
		dataString += "Linear acceleration: " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";
		
		vec = stage1.mMomentum;
		dataString += "Momentum:            " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";

		vec = stage1.mAngularVelocity;
		dataString += "Angular velocity:    " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";

		vec = stage1.mAngularMomentum;
		dataString += "Angular momentum:    " + to_string(vec.x) + " " + to_string(vec.y) + " " + to_string(vec.z) + "\n";

		dataString += "Total mass:          " + to_string(stage1.mTotalMass);

		mTextDrawList->AddText(ImVec2(0, 0), ImColor(1.0f, 1.0f, 1.0f), dataString.c_str());
	}

	void UILayer::simulationControls() const {
		using namespace Physics;
		using namespace ImGui;

		//temp
		Begin("Simulation controls", 0, ImGuiWindowFlags_AlwaysAutoResize);
		SliderFloat("Speed", &mSimSpeedHandle, 0.0f, 10.0f);
		
		if (Button("Pause")) 
			mSimSpeedHandle = 0.0f; 
		
		SameLine();
		if (Button("Resume")) 
			mSimSpeedHandle = 1.0f; 
		
		SameLine();
		if (Button("50%%")) 
			mSimSpeedHandle = 0.5f;

		End();
		//
	}

}
