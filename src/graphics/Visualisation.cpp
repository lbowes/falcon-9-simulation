#include "Visualisation.h"

#include <chrono_irrlicht/ChIrrWizard.h>
#include <chrono_irrlicht/ChIrrTools.h>
#include <core/ChVector.h>
#include <core/ChFrame.h>
#include <IMGUI/imgui.h>
#include <algorithm>
#include <iomanip>

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

	Visualisation::Visualisation(const std::string& simResultsFilepath_CSV, double KeyframeInterval_s, double simDuration_s) :
		mKeyframeInterval_s(KeyframeInterval_s),
		mSimDuration_s(simDuration_s),
		mIniSaveFilepath_imgui("../dat/Visualisation/imgui.ini"),
        mSaveStateFilepath("../dat/appState.json"),
        mSimResultsFilepath_CSV(simResultsFilepath_CSV),
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
        saveAppState();
	}

	Visualisation::~Visualisation() {
        mDevice->closeDevice();
        mDevice->run();
        mDevice->drop();
		ImGui::SaveIniSettingsToDisk(mIniSaveFilepath_imgui.c_str());
		mImGuiHandle->drop();

        nlohmann::json appState;
        save(appState);
        std::ofstream o(mSaveStateFilepath);
        o << std::setw(4) << appState;
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
		
		core::recti viewport = mDevice->getVideoDriver()->getViewPort();
		float aspectRatio = viewport.getWidth() / viewport.getHeight();

        // Instantiate and load the CameraSystem
		mCameraSystem = std::make_unique<CameraSystem>(*mDevice, *mSceneMgr, mHWinput, aspectRatio);
        mModel = std::make_unique<SimulationModel>(*mVidDriver, *mSceneMgr);

		ImGui::LoadIniSettingsFromDisk(mIniSaveFilepath_imgui.c_str());

        buildKeyframeHistory();

        loadAppState();
	}

    void Visualisation::buildKeyframeHistory() 
        // When the visualisation begins, it only has access to the output CSV file from the simulation.
        // This must first be converted into a series of key frames making up the animation history of the
        // system's model.
    {
        std::ifstream file(mSimResultsFilepath_CSV);
        
        if(!file) {
            printf("Visualisation::buildKeyframeHistory: Could not open file %s", mSimResultsFilepath_CSV.c_str());
            return;
        }

        unsigned frameIdx = 0; 

        std::string str;
        std::getline(file, str); // Skip first line of CSV file
        while (std::getline(file, str)) {
            // Each line of the CSV file is read into str - we need to parse this and use it to construct a Keyframe         
            mKeyframes.insert({frameIdx, Falcon9Keyframe(str)});
            frameIdx++;
        }
    }

	void Visualisation::handleInput(const float frameTime_s) {
		// Quit app with Esc
		if(mHWinput.isKeyPressed(irr::KEY_ESCAPE)) {
			mDevice->closeDevice();
            saveAppState();
        }

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
			s = floor(mPlayback.mTime_s / mKeyframeInterval_s),
			betweenKeyframes = fmod(mPlayback.mTime_s, mKeyframeInterval_s) / mKeyframeInterval_s;
	
		// Find the index of the the most recent snapshot to have been recorded...
		const unsigned
			KeyframeCount = mKeyframes.size(),
			recentKeyframeIdx = std::clamp(static_cast<unsigned>(s), 0U, static_cast<unsigned>(KeyframeCount - 1));

        if(!mKeyframes.empty()) {
		    Falcon9Keyframe
			    mostRecentFrame = mKeyframes.at(recentKeyframeIdx),
    			nextFrame = mKeyframes.at(std::clamp(recentKeyframeIdx + 1, 0U, static_cast<unsigned>(KeyframeCount - 1)));

		    mCurrentState = Falcon9Keyframe::lerp(mostRecentFrame, nextFrame, betweenKeyframes);;
        }
	}

    void Visualisation::saveAppState() const {
        nlohmann::json appState;
        save(appState);
        std::ofstream o(mSaveStateFilepath);
        o << std::setw(4) << appState;
    }

    void Visualisation::save(nlohmann::json& dest) const {
        mCameraSystem->save(dest["mCameraSystem"]);

        dest["mPlayback"]["mTime_s"] = mPlayback.mTime_s;
        dest["mPlayback"]["mSpeed"] = mPlayback.mSpeed;
        dest["mPlayback"]["mLastSpeed"] = mPlayback.mLastSpeed;
        dest["mPlayback"]["mPaused"] = mPlayback.mPaused;
    }
    
    void Visualisation::loadAppState() {
        std::ifstream saveState(mSaveStateFilepath);
        if(saveState) {
            std::stringstream buffer;
            buffer << saveState.rdbuf();
            load(buffer.str());
        }
    }

    void Visualisation::load(const std::string& source) {
        nlohmann::json j = nlohmann::json::parse(source);

        //j["mPlayback"]["mTime_s"].get_to(mPlayback.mTime_s);
        j["mPlayback"]["mSpeed"].get_to(mPlayback.mSpeed);
        j["mPlayback"]["mLastSpeed"].get_to(mPlayback.mLastSpeed);
        j["mPlayback"]["mPaused"].get_to(mPlayback.mPaused);

        mCameraSystem->load(j["mCameraSystem"].dump());
    }

}