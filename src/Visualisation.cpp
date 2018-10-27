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

	mDataSource.getState().setPosition_world({0.0, 0.0, 0.0});
	mDataSource.update(0.0, 0.0);

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

#if 0
	mSimTime_s += mPlaybackSpeed * mFrameTime;

	const std::map<const double, const DSS>::const_iterator prevState = mStateHistoryHandle.find(floor(mSimTime_s));

	const DSS
		&previousState = prevState->second,
		&nextState = std::next(prevState, 1)->second;

	double timeBetweenStates = (mSimTime_s - previousState.simulationTime) / (nextState.simulationTime - previousState.simulationTime);

	const DSS currentState = DSS::lerp(previousState, nextState, timeBetweenStates);
#endif

	//DSS currentState = mStateHistoryHandle.at(0.25);
	DSS currentState;
	currentState.F9.S1.thrusters[0].active = true;
	currentState.F9.S1.thrusters[1].active = true;
	
	//currentState.F9.S1.engines[0].throttle = 1.0;
	//currentState.F9.S1.engines[0].active = true;

	mDataSource.loadDynamicState(currentState.F9);

	//Use the approximation for the current state to render the simulation
	mSimulationModelLayer->render(mWindow.getAspect(), static_cast<float>(mFrameTime));
	//m2DOverlay->render(mSimulationModelLayer->getCameraSystem().getCurrentSimCamera().getViewProjection_generated(), mWindow.getAspect(), mWindow.getDimensions());
}