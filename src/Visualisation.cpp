#include "Visualisation.h"

Visualisation::Visualisation(const std::map<const double, const Physics::SimState>& stateHistoryHandle) :
	Application("Falcon 9 Simulation", "res/SpaceXLogo.png"),
	mStateHistoryHandle(stateHistoryHandle)
{
	onLoad();
}

void Visualisation::onLoad() {
	//Generate the state history using the data file
	//std::ifstream dataFile(mDataFilePath);
	//std::string line;

	//while(std::getline(dataFile, line)) {
	//	SimState state(line);
	//	mStateHistory.insert({ state.mSimulationTime, state });
	//}

	m2DOverlay = std::make_unique<Graphics::Overlay2D>(mPlaybackSpeed, mWindow.getAspect());
	mSimulationModelLayer = std::make_unique<Graphics::SimulationModelLayer>();

	mWindow.setClearColour(glm::vec4(0.259f, 0.435f, 0.588f, 1.0f));
}

void Visualisation::onInputCheck() {
	if (GF::Input::isKeyReleased(GLFW_KEY_ESCAPE)) 
		mRunning = false;
	
	if (GF::Input::isKeyReleased(GLFW_KEY_PAUSE)) {
		static float savedPlaybackSpeed = mPlaybackSpeed;
		if (mPlaybackSpeed != 0.0) {
			savedPlaybackSpeed = mPlaybackSpeed;
			mPlaybackSpeed = 0.0;
		}
		else
			mPlaybackSpeed = savedPlaybackSpeed;
	}

	mSimulationModelLayer->checkInput(static_cast<float>(mFrameTime), mWindow.getDimensions());
}

void Visualisation::onUpdate() { }

void Visualisation::onRender() {
	using namespace Physics;
	
	//Update the simulation time to a new point
	mSimTime_s += mPlaybackSpeed * mFrameTime;

	//Get an iterator to the previous state in the state history
	const std::map<const double, const SimState>::const_iterator prevState = mStateHistoryHandle.find(floor(mSimTime_s));

	//Get the previous and next simulation states to interpolate between
	const SimState& 
		previousState = prevState->second,
		nextState = std::next(prevState, 1)->second;

	//Work out what the simulation state should be at this new point
	double timeBetweenStates = (mSimTime_s - previousState.mSimulationTime) / (nextState.mSimulationTime - previousState.mSimulationTime);

	//Interpolate between the previous and next states to get the current state
	const SimState& currentState = SimState::lerp(previousState, nextState, timeBetweenStates);

	//Use the approximation for the current state to render the simulation
	mSimulationModelLayer->render(mWindow.getAspect(), static_cast<float>(mFrameTime));
	m2DOverlay->render(currentState.F9, mSimulationModelLayer->getCameraSystem().getCurrentSimCamera().getViewProjection_generated(), mWindow.getAspect(), mWindow.getDimensions());
}