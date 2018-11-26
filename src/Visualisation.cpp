#include "Visualisation.h"

#include <GraphicsFramework/Vendor/ImGui/imgui.h>

Visualisation::Visualisation(const std::map<const unsigned, const Physics::DSS>& stateHistoryHandle, double snapshotInterval_s) :
	Application("Falcon 9 Simulation", "res/SpaceXLogo.png"),
	mStateHistory(stateHistoryHandle),
	mSnapshotInterval_s(snapshotInterval_s)
{
	onLoad();
}

void Visualisation::onLoad() {
	float windowAspect = mWindow.getAspect();
	m2DOverlay = std::make_unique<Graphics::Overlay2D>(mDataSource, mPlaybackSpeed, mSimTime_s, windowAspect);
	mSimulationModelLayer = std::make_unique<Graphics::SimulationModelLayer>(mDataSource, windowAspect);

	mDataSource.getState().setPosition_world({ 0.0, 0.0, 0.0 });
	mDataSource.update(0.0, 0.0);

	mWindow.setClearColour({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void Visualisation::onInputCheck() {
	if (GF::Input::isKeyClicked(GLFW_KEY_ESCAPE)) 
		mRunning = false;
	
	if (GF::Input::isKeyClicked(GLFW_KEY_PAUSE)) {
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
	//Advance the simulation time correctly according to the frame time...
	mSimTime_s += mPlaybackSpeed * mFrameTime;

	//Localise the current time within the snapshot history...
	double 
		s = floor(mSimTime_s / mSnapshotInterval_s),
		betweenSnapshots = fmod(mSimTime_s, mSnapshotInterval_s) / mSnapshotInterval_s;

	//Find the index of the the most recent snapshot to have been recorded...
	unsigned 
		snapshotCount = mStateHistory.size(),
		recentSnapshotNum = std::clamp(static_cast<unsigned>(s), 0U, static_cast<unsigned int>(snapshotCount - 1));

	//Find the a) most recent snapshot, b) the next snapshot to come and c) a linear interpolation between the two...
	Physics::DSS 
		mostRecentState = mStateHistory.at(recentSnapshotNum),
		nextState = mStateHistory.at(std::clamp(recentSnapshotNum + 1, 0U, static_cast<unsigned int>(snapshotCount - 1))),
		interpolated;

	Physics::DSS::lerp(mostRecentState, nextState, betweenSnapshots, interpolated);

	//Load the dummy Falcon9 object with this interpolated state ready for rendering...
	mDataSource.loadDynamicState(interpolated.F9);

	//Render the dummy Falcon9 object 
	mSimulationModelLayer->render(mWindow.getAspect(), static_cast<float>(mFrameTime));
	m2DOverlay->render(mSimulationModelLayer->getCameraSystem().getCurrentSimCamera().getViewProjection_generated(), mWindow.getAspect(), mWindow.getDimensions(), mSnapshotInterval_s * snapshotCount);
}