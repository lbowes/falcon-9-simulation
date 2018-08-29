//This class is responsible for everything to do with rendering the simulation. The physical simulation itself
//should be completely isolated from the visuals of the program. All models/shaders/cameras etc should be 
//managed in here.
#ifndef GRAPHICS_GRAPHICALSHELL
#define GRAPHICS_GRAPHICALSHELL
#pragma once

#include "Graphics/Simulation_model/SimulationModelLayer.h"
#include "Graphics/Overlay/Overlay2D.h"

#include <GraphicsFramework/Window.h>

namespace Graphics {
	
	class GraphicalShell {
	private:
		GF::Window& mWindow;
		Overlay2D m2DOverlay;
		std::unique_ptr<SimulationModelLayer> mSimulationModelLayer;

	public:
		GraphicalShell(Physics::Hardware::Falcon9& simDataSource, GF::Window& window, float& simulationSpeedHandle);
		~GraphicalShell() = default;

		void renderAll(float dt);
		void checkInput(float dt);

	private:
		void load();

	};

}

#endif
