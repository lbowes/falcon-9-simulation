#include "GraphicalShell.h"
#include "Physics/Hardware/Falcon_9/Falcon9.h"

namespace Graphics {

	GraphicalShell::GraphicalShell(Physics::Hardware::Falcon9& simDataSource, GF::Window& window, float& simulationSpeedHandle) :
		mWindow(window),
		m2DOverlay(simDataSource, simulationSpeedHandle, window.getAspect()),
		mSimulationModelLayer(std::make_unique<SimulationModelLayer>(simDataSource, window.getAspect()))
	{
		load();
	}

	void GraphicalShell::renderAll(float dt) {
		mSimulationModelLayer->render(mWindow.getAspect(), dt);
		m2DOverlay.render(mSimulationModelLayer->getCameraSystem().getCurrentSimCamera().getViewProjection_generated(), mWindow.getAspect(), mWindow.getDimensions());
	}

	void GraphicalShell::checkInput(float dt) {
		mSimulationModelLayer->checkInput(dt, mWindow.getDimensions());
	}

	void GraphicalShell::load() {
		mWindow.setClearColour(glm::vec4(0.259f, 0.435f, 0.588f, 1.0f));
	}

}