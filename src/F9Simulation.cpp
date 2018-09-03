#include "F9Simulation.h"

#include <unistd.h>

F9Simulation::F9Simulation() :
	Application("Falcon 9 Simulation", "res/SpaceXLogo.png")
{
	onLoad();
}
									   
void F9Simulation::onLoad() {
	glm::dvec3 vehiclePosition = { 0.0, 3.0, 0.0 };
	glm::dmat4 vehicleRotation = glm::rotate(glm::radians(0.0), glm::dvec3(1.0, 0.0, 0.0));

	mFalcon9.mutableState().setObjectToParentTransform({ vehiclePosition, vehicleRotation });
	
	mGraphics = std::make_unique<Graphics::GraphicalShell>(mFalcon9, mWindow, mSimulationSpeed);
}

void F9Simulation::onInputCheck() {
	if (GF::Input::isKeyReleased(GLFW_KEY_ESCAPE)) 
		mRunning = false;
	
	if (GF::Input::isKeyReleased(GLFW_KEY_PAUSE)) {
		static float savedSimSpeed = mSimulationSpeed;
		if (mSimulationSpeed != 0.0) {
			savedSimSpeed = mSimulationSpeed;
			mSimulationSpeed = 0.0;
		}
		else
			mSimulationSpeed = savedSimSpeed;
	}

	mFalcon9.checkInput(static_cast<float>(mFrameTime));
	mGraphics->checkInput(static_cast<float>(mFrameTime));
}

void F9Simulation::onUpdate() {
	if(mSimulationSpeed > 0.0)
		mFalcon9.update(mCurrentTime, mUpdateDelta * static_cast<double>(mSimulationSpeed));
}

void F9Simulation::onRender() {
	mGraphics->renderAll(static_cast<float>(mFrameTime));
}