#ifndef GRAPHICS_UILAYER_H
#define GRAPHICS_UILAYER_H
#pragma once

//testing graphs
#include "Graph.h"
//
#include "Physics/DynamicSimState.h"

#include <GraphicsFramework/Vendor/ImGui/imgui.h>

namespace Physics {
	namespace Hardware {
		class Falcon9;
	}
}

namespace Graphics {
	
	class UILayer {
	private:
		Graph 
			stage1_testGraph,
			stage2_testGraph;
		
		const Physics::Hardware::Falcon9& mDataSource;
		float& mSimSpeedHandle;

		ImDrawList* mTextDrawList = nullptr;

	public:
		UILayer(const Physics::Hardware::Falcon9& simDataSource, float& playbackSpeedHandle);
		~UILayer() = default;

		void render(glm::vec2 mainWindowSize);

	private:
		void load() const;
		void stage1PhysicsState() const;
		void simulationControls() const;

	};

}

#endif
