#include "UILayer.h"
#include "Physics/Hardware/Falcon_9/Falcon9.h"

#include <sstream>
#include <iomanip>

namespace Graphics {
	
	UILayer::UILayer(Physics::Hardware::Falcon9& simDataSource, float& simulationSpeedHandle) :
		mDataSource(simDataSource),
		mSimulationSpeedHandle(simulationSpeedHandle)
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

	void UILayer::render(glm::vec2 mainWindowSize) {
		mTextDrawList = ImGui::GetOverlayDrawList();

		stage1PhysicsState();
		//stage1Propellant(115.0f);
		//stage1ExtraInfo(115.0f);
		simulationControls();

		//temp testing graph
		const State&
			stage1State = mDataSource.getStage1().getState(),
			stage2State = mDataSource.getStage2().getState();
		
		static double lastTime = 0.0;
		if (stage1State.getCMPosition_world().y > 0.0) {
			double
				currentTime = glfwGetTime(),
				delta = currentTime - lastTime;

			if (delta > 0.5) {
				stage1_testGraph.addDataPoint({ static_cast<float>(stage1State.getCMPosition_world().z), static_cast<float>(stage1State.getCMPosition_world().y) });
				stage2_testGraph.addDataPoint({ static_cast<float>(stage2State.getCMPosition_world().z), static_cast<float>(stage2State.getCMPosition_world().y) });

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
#if 0
		using namespace ImGui;
		
		State& stage1State = mDataSource.getStage1().getState();
		
		Begin("Stage_1 Overview");
		Text("Physics state");
		BeginChild("State", ImVec2(0.0f, verticalSize), true);
		{
			Columns(2);
			glm::dvec3 temp = stage1State.getCMPosition_world();
			Text("Position\nx: %.10f\ny: %.10f\nz: %.10f", temp.x, temp.y, temp.z);

			NextColumn();
			temp = stage1State.getVelocity_world();
			Text("Linear Velocity\nx: %.10f\ny: %.10f\nz: %.10f", temp.x, temp.y, temp.z); Separator();
			
			NextColumn();
			temp = mDataSource.getStage1().getAccel_world();
			Text("Linear Acceleration\nx: %.10f\ny: %.10f\nz: %.10f", temp.x, temp.y, temp.z);

			NextColumn();
			temp = stage1State.getMomentum_world();
			Text("Momentum\nx: %.10f\ny: %.10f\nz: %.10f", temp.x, temp.y, temp.z); Separator();

			NextColumn();
			temp = stage1State.getAngularVelocity_world();
			Text("Angular Velocity\nx: %.10f\ny: %.10f\nz: %.10f", temp.x, temp.y, temp.z);

			NextColumn();
			temp = stage1State.getAngularMomentum_world();
			Text("Angular Momentum\nx: %.10f\ny: %.10f\nz: %.10f", temp.x, temp.y, temp.z);
		}
		EndChild();

		Text("Total mass: %.3f", stage1State.getMass_local().getValue());
		
		End();
#else
		using namespace std;

		State& stage1State = mDataSource.getStage1().getState();
		
		string dataString = "";
		
		glm::dvec3 temp = stage1State.getCMPosition_world();
		dataString += "Position: " + to_string(temp.x) + " " + to_string(temp.y) + " " + to_string(temp.z) + "\n";

		temp = stage1State.getVelocity_world();
		dataString += "Velocity: " + to_string(temp.x) + " " + to_string(temp.y) + " " + to_string(temp.z) + "\n";

		temp = mDataSource.getStage1().getAccel_world();
		dataString += "Linear acceleration: " + to_string(temp.x) + " " + to_string(temp.y) + " " + to_string(temp.z) + "\n";
		
		temp = stage1State.getMomentum_world();
		dataString += "Momentum: " + to_string(temp.x) + " " + to_string(temp.y) + " " + to_string(temp.z) + "\n";

		temp = stage1State.getAngularVelocity_world();
		dataString += "Angular Velocity: " + to_string(temp.x) + " " + to_string(temp.y) + " " + to_string(temp.z) + "\n";

		temp = stage1State.getAngularMomentum_world();
		dataString += "Angular Momentum: " + to_string(temp.x) + " " + to_string(temp.y) + " " + to_string(temp.z) + "\n";

		dataString += "Mass: " + to_string(stage1State.getMass_local().getValue());

		mTextDrawList->AddText(ImVec2(0, 0), ImColor(1.0f, 1.0f, 1.0f), dataString.c_str());
#endif
	}

#if 0
	void UILayer::stage1Propellant() const {
		using namespace ImGui;
		using namespace Physics;
		using namespace Physics::Hardware;

		const FluidTankGroup& tanks = mDataSource.getStage1().getPropellantSupplies();
		unsigned propMassRemaining = 0;
		float propPercentRemaining = 0.0f;
		std::stringstream ss;
		std::string overlay;

		Begin("Stage_1 Overview");
		Text("Propellant");
		BeginChild("Propellant", ImVec2(0.0f, verticalSize), true);
		{
			//Liquid Oxygen
			{
				propMassRemaining = static_cast<int>(floor(static_cast<const FluidTank*>(tanks[Propellants::liquidOxygen])->getPropMassValue_tank()));
				propPercentRemaining = static_cast<float>(static_cast<const FluidTank*>(tanks[Propellants::liquidOxygen])->getPercentFull());
				ss << std::fixed << std::setprecision(2) << propPercentRemaining * 100;
				overlay = "LOX: " + ss.str() + "% " + std::to_string(propMassRemaining) + "kg";
				PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.4156862745098039f, 0.4274509803921569f, 0.603921568627451f, 1.0f));
				ProgressBar(propPercentRemaining, ImVec2(-1.0, 30.0), overlay.c_str());
				PopStyleColor();
			}

			//RP-1
			{
				propMassRemaining = static_cast<int>(floor(static_cast<const FluidTank*>(tanks[Propellants::RP1])->getPropMassValue_tank()));
				propPercentRemaining = static_cast<float>(static_cast<const FluidTank*>(tanks[Propellants::RP1])->getPercentFull());
				ss.str(std::string());
				ss << propPercentRemaining * 100;
				overlay = "RP-1: " + ss.str() + "% " + std::to_string(propMassRemaining) + "kg";
				PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.5725490196078431f, 0.6313725490196078f, 0.4784313725490196f, 1.0f));
				ProgressBar(propPercentRemaining, ImVec2(-1.0, 30.0), overlay.c_str());
				PopStyleColor();
			}

			//Nitrogen gas
			{
				propMassRemaining = static_cast<int>(floor(static_cast<const FluidTank*>(tanks[Propellants::nitrogen])->getPropMassValue_tank()));
				propPercentRemaining = static_cast<float>(static_cast<const FluidTank*>(tanks[Propellants::nitrogen])->getPercentFull());
				ss.str(std::string());
				ss << propPercentRemaining * 100;
				overlay = "Nitrogen: " + ss.str() + "% " + std::to_string(propMassRemaining) + "kg";
				PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
				ProgressBar(propPercentRemaining, ImVec2(-1.0, 30.0), overlay.c_str());
				PopStyleColor();
			}
		}
		EndChild();
		End();
	}

	void UILayer::stage1ExtraInfo() const {
		using namespace ImGui;
		
		Begin("Stage_1 Overview");
		Text("Motion");
		BeginChild("Motion", ImVec2(0.0f, verticalSize), true);
		{
			float speed = static_cast<float>(glm::length(mDataSource.getStage1().getState().getVelocity_world()));
			Text("Speed\nm/h: %.3f\nk/h: %.3f\nm/s: %.3f", speed * 2.23694, speed * 3.6, speed); Separator();

			//float 
			//	angleAttackPitch = mSimulationDataSource.getAngleAttack(true),
			//	angleAttackAirflow = mSimulationDataSource.getAngleAttack(false);
			//Text("Angle of attack\nPitch:    %.3f\nAir flow: %.3f", angleAttackPitch, angleAttackAirflow);
		}
		EndChild();
		End();
	}
#endif

	void UILayer::simulationControls() const {
		using namespace Physics;
		using namespace ImGui;

		//temp
		Begin("Simulation controls", 0, ImGuiWindowFlags_AlwaysAutoResize);
		SliderFloat("Speed", &mSimulationSpeedHandle, 0.0f, 10.0f);
		
		if (Button("Pause")) 
			mSimulationSpeedHandle = 0.0f; 
		
		SameLine();
		if (Button("Resume")) 
			mSimulationSpeedHandle = 1.0f; 
		
		SameLine();
		if (Button("50%%")) 
			mSimulationSpeedHandle = 0.5f;

		if (Button("Deploy grid fins")) {
			for (const auto& fin : mDataSource.getStage1().getGridFins().getAllComponents())
				static_cast<Physics::Hardware::GridFin*>(fin.get())->deploy();
		}
		SameLine();
		if (Button("Stow grid fins")) {
			for (const auto& fin : mDataSource.getStage1().getGridFins().getAllComponents())
				static_cast<Physics::Hardware::GridFin*>(fin.get())->stow();
		}

		if (Button("Deploy landing legs")) {
			for (const auto& leg : mDataSource.getStage1().getLandingLegs().getAllComponents())
				static_cast<Physics::Hardware::LandingLeg*>(leg.get())->deploy();
		}
		SameLine();
		if (Button("Stow landing legs")) {
			for (const auto& leg : mDataSource.getStage1().getLandingLegs().getAllComponents())
				static_cast<Physics::Hardware::LandingLeg*>(leg.get())->stow_temp();
		}

		if (Button("Drain all propellant")) {
			const auto& tanks = mDataSource.getStage1().getPropellantSupplies().getAllComponents();
			static_cast<Physics::Hardware::FluidTank*>(tanks[Propellants::liquidOxygen].get())->removeAllPropellant();
			static_cast<Physics::Hardware::FluidTank*>(tanks[Propellants::RP1].get())->removeAllPropellant();
			static_cast<Physics::Hardware::FluidTank*>(tanks[Propellants::nitrogen].get())->removeAllPropellant();
		}
		SameLine();
		if (Button("Replenish all propellant")) {
			const auto& tanks = mDataSource.getStage1().getPropellantSupplies().getAllComponents();
			static_cast<Physics::Hardware::FluidTank*>(tanks[Propellants::liquidOxygen].get())->addPropellant(1000000.0);
			static_cast<Physics::Hardware::FluidTank*>(tanks[Propellants::RP1].get())->addPropellant(1000000.0);
			static_cast<Physics::Hardware::FluidTank*>(tanks[Propellants::nitrogen].get())->addPropellant(1000000.0);
		}

		End();
		//
	}

}
