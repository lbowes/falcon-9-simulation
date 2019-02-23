#include "Visualisation.h"
#include "../Physics/Internal/Hardware/Falcon_9/F9.h"

#include <chrono_irrlicht/ChIrrWizard.h>
#include <chrono_irrlicht/ChIrrTools.h>
#include <algorithm>
#include <core/ChVector.h>

#include <IMGUI/imgui.h>

namespace Graphics {

	PlaybackConfig::PlaybackConfig() :
		mTime_s(0),
		mSpeed(1),
		mLastSpeed(1),
		mPaused(false)
	{ }

	Visualisation::Visualisation(const std::map<const unsigned, const Physics::F9_DSS>& stateHistoryHandle, double snapshotInterval_s, double simDuration) :
		mStateHistory(stateHistoryHandle),
		mSnapshotInterval_s(snapshotInterval_s),
		mSimDuration(simDuration)
	{
		using namespace irr;
		
		mEventReceiver.addReciever(&mHWinput);
		mEventReceiver.addReciever(&mImGuiEventReceiver);

		//Get monitor resolution
		IrrlichtDevice* tmp = createDevice(video::EDT_NULL);
		core::dimension2du res = tmp->getVideoModeList()->getDesktopResolution();
		tmp->drop();

		SIrrlichtCreationParameters params;
		params.DriverType = video::EDT_OPENGL;
		params.WindowSize = res / 2;
		params.Bits = 16;
		params.Fullscreen = false;
		params.Stencilbuffer = true;
		params.Vsync = false;
		params.EventReceiver = &mEventReceiver;

		mDevice = createDeviceEx(params);

		if(!mDevice) {
			printf("Error creating device\n");
			std::cin.get();
			return;
		}

		mDevice->setWindowCaption(L"Falcon 9 Simulation");

		//Done to prevent IrrIMGUI from hiding the OS cursor during initialisation and forcing a new rendered cursor
		IrrIMGUI::SIMGUISettings settings;
		settings.mIsGUIMouseCursorEnabled = false;

		mImGuiHandle = IrrIMGUI::createIMGUI(mDevice, &mImGuiEventReceiver, &settings);

		mVidDriver = mDevice->getVideoDriver();
		mSceneMgr = mDevice->getSceneManager();
		
		init();
	}

	Visualisation::~Visualisation() {
		mDevice->drop();
		mImGuiHandle->drop();
	}

	void Visualisation::run() {
		unsigned lastTime = mDevice->getTimer()->getRealTime();

		while(mDevice->run()) {
        	const unsigned now = mDevice->getTimer()->getRealTime();
        	const float frameTime_s = static_cast<float>(now - lastTime) / 1000.0f;
	        lastTime = now;

			mImGuiHandle->startGUI();
			handleInput(frameTime_s);
			update(frameTime_s);
			render();
		}
	}

	void Visualisation::init() {
		using namespace irr;
		
		mDevice->getCursorControl()->setVisible(false);

		core::recti viewport = mDevice->getVideoDriver()->getViewPort();
		float aspectRatio = viewport.getWidth() / viewport.getHeight();

		mCameraSystem = std::make_unique<CameraSystem>(*mDevice, *mSceneMgr, mHWinput, aspectRatio);
		mModelLayer = std::make_unique<SimulationModelLayer>(*mVidDriver, *mSceneMgr, aspectRatio);
		mGUILayer = std::make_unique<GUI::GUILayer>(mPlayback, mSimDuration);
	}

	void Visualisation::handleInput(const float frameTime_s) {
		if(mHWinput.isKeyPressed(irr::KEY_ESCAPE))
			mDevice->closeDevice();

		const irr::core::recti dims = mVidDriver->getViewPort();
		irr::core::vector2di centreScreen = irr::core::vector2di(dims.getWidth() / 2, dims.getHeight() / 2);
		mCameraSystem->handleInput(centreScreen, frameTime_s);
	}

	void Visualisation::update(float frameTime_s) {
		const irr::core::recti dims = mVidDriver->getViewPort();
		irr::core::vector2di centreScreen = irr::core::vector2di(dims.getWidth() / 2, dims.getHeight() / 2);
		mHWinput.update(centreScreen);

		//Fix window aspect ratio with resizing
		const float aspectRatio = static_cast<float>(dims.getWidth()) / dims.getHeight();
		mCameraSystem->update(aspectRatio, frameTime_s);
		
		handleTimeSelection(frameTime_s);

		//Pass in the required camera position so that models can update their own positions
		mModelLayer->update(mCameraSystem->getCurrentSimCamera().getPosition_world(), mLiveSnapshot, frameTime_s);
	}
	
	void Visualisation::render() {
		mVidDriver->beginScene(true, true, irr::video::SColor(255, 0, 0, 0));
		mGUILayer->render();
		
		mModelLayer->render(mCameraSystem->getCurrentSimCamera().getPosition_world());
		
		mImGuiHandle->drawAll();
		mVidDriver->endScene();
	}

	void Visualisation::handleTimeSelection(float frameTime_s) {
		// Advance the simulation time correctly according to the frame time...
		mPlayback.mTime_s = std::min(static_cast<float>(mSimDuration), mPlayback.mTime_s + mPlayback.mSpeed * frameTime_s);
		
		// Localise the current time within the snapshot history...
		double 
			s = floor(mPlayback.mTime_s / mSnapshotInterval_s),
			betweenSnapshots = fmod(mPlayback.mTime_s, mSnapshotInterval_s) / mSnapshotInterval_s;
	
		// Find the index of the the most recent snapshot to have been recorded...
		const unsigned
			snapshotCount = mStateHistory.size(),
			recentSnapshotNum = std::clamp(static_cast<unsigned>(s), 0U, static_cast<unsigned>(snapshotCount - 1));

		Physics::F9_DSS
			mostRecentState = mStateHistory.at(recentSnapshotNum),
			nextState = mStateHistory.at(std::clamp(recentSnapshotNum + 1, 0U, static_cast<unsigned>(snapshotCount - 1)));

		mLiveSnapshot = Physics::F9_DSS::lerp(mostRecentState, nextState, betweenSnapshots);;
	}

}