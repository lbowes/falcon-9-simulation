#include "Visualisation.h"

#include <chrono_irrlicht/ChIrrWizard.h>
#include <chrono_irrlicht/ChIrrTools.h>
#include <core/ChVector.h>
#include <core/ChFrame.h>
#include <IMGUI/imgui.h>
#include <algorithm>

namespace Graphics {

	PlaybackConfig::PlaybackConfig() :
		mTime_s(0),
		mSpeed(1),
		mLastSpeed(1),
		mPaused(false)
	{ }

    void PlaybackConfig::togglePauseState() {
		if(mPaused) {
			mSpeed = mLastSpeed;
			mPaused = false;
		}
		else {
			mLastSpeed = mSpeed;
			mSpeed = 0.0f;
			mPaused = true;
		}
	}

	Visualisation::Visualisation(double keyFrameInterval_s, double simDuration_s) :
		mKeyFrameInterval_s(keyFrameInterval_s),
		mSimDuration_s(simDuration_s),
		mIniSaveFile_imgui("../dat/Visualisation/imgui.ini"),
        mGUILayer(mPlayback, simDuration_s)
	{
		using namespace irr;
		
        // Initialise device
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

        // Initialise other core Irrlicht components
        mVidDriver = mDevice->getVideoDriver();
		mSceneMgr = mDevice->getSceneManager();

		mEventReceiver.addReciever(&mHWinput);
		mEventReceiver.addReciever(&mImGuiEventReceiver);

		// Prevents IrrIMGUI from hiding the OS cursor during initialisation and forcing a new rendered cursor
		IrrIMGUI::SIMGUISettings settings;
		settings.mIsGUIMouseCursorEnabled = false;

        mImGuiHandle = IrrIMGUI::createIMGUI(mDevice, &mImGuiEventReceiver, &settings);

        // Must be called after ImGui is initialised
        mGUILayer.loadImGuiStyle();        

		init();
        run();
	}

	Visualisation::~Visualisation() {
		mDevice->drop();
		ImGui::SaveIniSettingsToDisk(mIniSaveFile_imgui.c_str());
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

    // TODO: Consider implementing application state saving similar to SNSS (window position/size)
    // This could all be done with two new methods in here: save() and load()

	void Visualisation::init() {
		using namespace irr;
		
		mDevice->getCursorControl()->setVisible(false);

		core::recti viewport = mDevice->getVideoDriver()->getViewPort();
		float aspectRatio = viewport.getWidth() / viewport.getHeight();

		mCameraSystem = std::make_unique<CameraSystem>(*mDevice, *mSceneMgr, mHWinput, aspectRatio);
		mModel = std::make_unique<SimulationModel>(*mVidDriver, *mSceneMgr);

		ImGui::LoadIniSettingsFromDisk(mIniSaveFile_imgui.c_str());

        buildKeyFrameHistory();
	}

    void Visualisation::buildKeyFrameHistory() 
        // When the visualisation begins, it only has access to the output CSV file from the simulation.
        // This must first be converted into a series of key frames making up the animation history of the
        // system's model.
    {
        // mKeyFrames.insert(..., ...);
    }

	void Visualisation::handleInput(const float frameTime_s) {
		// Quit app with Esc
		if(mHWinput.isKeyPressed(irr::KEY_ESCAPE))
			mDevice->closeDevice();

        // Pause/resume with keyboard
		if(mHWinput.isKeyReleased(irr::KEY_PAUSE))
			mPlayback.togglePauseState();

        // Camera control input
		const irr::core::recti dims = mVidDriver->getViewPort();
		irr::core::vector2di centreScreen = irr::core::vector2di(dims.getWidth() / 2, dims.getHeight() / 2);
		mCameraSystem->handleInput(centreScreen, frameTime_s);
	}

	void Visualisation::update(float frameTime_s) {
		const irr::core::recti dims = mVidDriver->getViewPort();
		irr::core::vector2di centreScreen = irr::core::vector2di(dims.getWidth() / 2, dims.getHeight() / 2);
		mHWinput.update(centreScreen);

        // Update the point in time being displayed by the simulation
        handleTimeSelection(frameTime_s);

        // Fix window aspect ratio with resizing
		const float aspectRatio = static_cast<float>(dims.getWidth()) / dims.getHeight();
		
        // todo: fix this, the camera system should be given the transform of the interstage camera mount
        // Shouldn't the interstage camera have its own ChCoordsys which could then be used to have
        // ChIrrTools align the camera to the model automatically somehow?
        mCameraSystem->update(chrono::ChCoordsys<double>(), aspectRatio, frameTime_s);
        //mCameraSystem->update(mCurrentState.getF9S1_DSS().getS1ToWorldTransform().GetCoord(), aspectRatio, frameTime_s);
		
        // Pass in the required camera position so that models can update their own positions
		mModel->update(mCameraSystem->getCurrentSimCamera().getPosition_world(), mCurrentState);
	}
	
	void Visualisation::render() {
		mVidDriver->beginScene(true, true, irr::video::SColor(255, 0, 0, 0));
		
		mGUILayer.render();
		mModel->render(mCameraSystem->getCurrentSimCamera().getPosition_world());
		mImGuiHandle->drawAll();
		
		mVidDriver->endScene();
	}

	void Visualisation::handleTimeSelection(float frameTime_s) {
		// Advance the simulation time correctly according to the frame time...
		mPlayback.mTime_s = std::min(static_cast<float>(mSimDuration_s), mPlayback.mTime_s + mPlayback.mSpeed * frameTime_s);
		
		// Localise the current time within the snapshot history...
		double 
			s = floor(mPlayback.mTime_s / mKeyFrameInterval_s),
			betweenKeyFrames = fmod(mPlayback.mTime_s, mKeyFrameInterval_s) / mKeyFrameInterval_s;
	
		// Find the index of the the most recent snapshot to have been recorded...
		const unsigned
			keyFrameCount = mKeyFrames.size(),
			recentKeyFrameIdx = std::clamp(static_cast<unsigned>(s), 0U, static_cast<unsigned>(keyFrameCount - 1));

        if(!mKeyFrames.empty()) {
		    ModelKeyFrame
			    mostRecentState = mKeyFrames.at(recentKeyFrameIdx),
    			nextState = mKeyFrames.at(std::clamp(recentKeyFrameIdx + 1, 0U, static_cast<unsigned>(keyFrameCount - 1)));

		    mCurrentState = ModelKeyFrame::lerp(mostRecentState, nextState, betweenKeyFrames);;
        }
	}

}