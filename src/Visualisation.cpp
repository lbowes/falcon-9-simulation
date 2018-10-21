#include "Visualisation.h"

Visualisation::Visualisation(const std::map<const double, const Physics::DSS>& stateHistoryHandle) :
	Application("Falcon 9 Simulation", "res/SpaceXLogo.png"),
	mStateHistoryHandle(stateHistoryHandle)
{
	onLoad();
}

void Visualisation::onLoad() {
	float windowAspect = mWindow.getAspect();
	m2DOverlay = std::make_unique<Graphics::Overlay2D>(mDataSource, mPlaybackSpeed, windowAspect);
	mSimulationModelLayer = std::make_unique<Graphics::SimulationModelLayer>(mDataSource, windowAspect);

	mWindow.setClearColour({ 0.0f, 0.0f, 0.0f, 1.0f });
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
//	mSimTime_s += mPlaybackSpeed * mFrameTime;
//
//	//Get an iterator to the previous state in the state history
//	const std::map<const double, const DSS>::const_iterator prevState = mStateHistoryHandle.find(floor(mSimTime_s));
//
//	//Get the previous and next simulation states to interpolate between
//	const DSS& 
//		previousState = prevState->second,
//		nextState = std::next(prevState, 1)->second;
//
//	//Work out what the simulation state should be at this new point
//	double timeBetweenStates = (mSimTime_s - previousState.simulationTime) / (nextState.simulationTime - previousState.simulationTime);
//
//	//Interpolate between the previous and next states to get the current state
//	const DSS currentState = DSS::lerp(previousState, nextState, timeBetweenStates);

	//Use the approximation for the current state to render the simulation
	mSimulationModelLayer->render(mWindow.getAspect(), static_cast<float>(mFrameTime));
	m2DOverlay->render(mSimulationModelLayer->getCameraSystem().getCurrentSimCamera().getViewProjection_generated(), mWindow.getAspect(), mWindow.getDimensions());
}