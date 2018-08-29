#include "F9Simulation.h"

#include <unistd.h>

F9Simulation::F9Simulation() :
	Application("Falcon 9 Simulation", "res/SpaceXLogo.png")
{
	onLoad();
}
									   
void F9Simulation::onLoad() {
	glm::dvec3 vehiclePosition = { 0.0, 3.0, 0.0 };
	glm::dquat vehicleRotation = glm::rotate(glm::dquat(), glm::radians(0.0), { 1.0, 0.0, 0.0 });

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

	//Mouse-controlled engine gimballing
	//for (const auto& p : mFalcon9.getStage1().getEngines().getAllComponents()) {
	//	static_cast<Physics::Hardware::Engine*>(p.get())->getActuators()[0]->setPercentExtension(1.0f - (mWindow.getDimensions().y - GF::Input::getMousePosition().y) / mWindow.getDimensions().y);
	//	static_cast<Physics::Hardware::Engine*>(p.get())->getActuators()[1]->setPercentExtension((mWindow.getDimensions().x - GF::Input::getMousePosition().x) / mWindow.getDimensions().x);
	//}

	//Sinusodial engine gimballing oscillation over time
	//for (const auto& p : mFalcon9.getStage1().getEngines().getAllComponents()) {
	//	static_cast<Physics::Hardware::Engine*>(p.get())->getActuators()[0]->setPercentExtension((sin(glfwGetTime() * 4) + 1) / 2);
	//	static_cast<Physics::Hardware::Engine*>(p.get())->getActuators()[1]->setPercentExtension((sin(glfwGetTime() * 4 + (M_PI / 2)) + 1) / 2);
	//}

	//static int mode = GL_FILL;
	//if (GF::Input::isKeyReleased(GLFW_KEY_0)) {
	//	mode = mode == GL_LINE ? GL_FILL : GL_LINE;
	//	glPolygonMode(GL_FRONT_AND_BACK, mode);
	//}

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