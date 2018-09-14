#ifndef GRAPHICS_UILAYER_H
#define GRAPHICS_UILAYER_H
#pragma once

//testing graphs
#include "Graph.h"
//
#include "Physics/DynamicSimState.h"

#include <GraphicsFramework/Vendor/ImGui/imgui.h>

namespace Graphics {
	
	class UILayer {
	private:
		Graph 
			stage1_testGraph,
			stage2_testGraph;
		
		float& mSimSpeedHandle;

		ImDrawList* mTextDrawList = nullptr;

	public:
		UILayer(float& playbackSpeedHandle);
		~UILayer() = default;

		void render(const Physics::DynamicSimState::Falcon9::Stage1& stage1, glm::vec2 mainWindowSize);

	private:
		void load() const;
		void stage1PhysicsState(const Physics::DynamicSimState::Falcon9::Stage1& stage1) const;
		void simulationControls() const;

	};

}

#endif
