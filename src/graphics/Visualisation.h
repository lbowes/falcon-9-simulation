#ifndef F9SIM_GRAPHICS_VISUALISATION_H_
#define F9SIM_GRAPHICS_VISUALISATION_H_
#pragma once

#include "../input/MasterEventReceiver.h"
#include "../input/HWEventReceiver.h"
#include "gui/GUILayer.h"
#include "simulation_model/cameras/CameraSystem.h"
#include "simulation_model/SimulationModel.h"
#include "simulation_model/Falcon9Keyframe.h"
#include "ISerialisable.hpp"

#include <IrrIMGUI/IrrIMGUI.h>
#include <map>

namespace irr {
	class IrrlichtDevice;

	namespace video {
		class IVideoDriver;
	}

	namespace scene {
		class ISceneManager;
	}
}

namespace Graphics {

	struct PlaybackConfig {
		float
			mTime_s,
			mSpeed,
			mLastSpeed;

		bool mPaused;

		PlaybackConfig();
		~PlaybackConfig() = default;

		void togglePauseState();
	};

	class Visualisation : public ISerialisable {
	private:
		const double
			mKeyframeInterval_s,
			mSimDuration_s;

		const std::string
            mIniSaveFilepath_imgui,
            mSaveStateFilepath,
            mSimResultsFilepath_CSV;

	    IrrIMGUI::IIMGUIHandle* mImGuiHandle;
        irr::IrrlichtDevice* mDevice;
		irr::video::IVideoDriver* mVidDriver;
    	irr::scene::ISceneManager* mSceneMgr;

        PlaybackConfig mPlayback;
		Input::HWEventReceiver mHWinput;
		Input::MasterEventReceiver mEventReceiver;
		IrrIMGUI::CIMGUIEventReceiver mImGuiEventReceiver;

		GUI::GUILayer mGUILayer;
		std::unique_ptr<CameraSystem> mCameraSystem;
		std::unique_ptr<SimulationModel> mModel;

		std::map<const unsigned, const Falcon9Keyframe> mKeyframes;

	 	Falcon9Keyframe mCurrentState;

	public:
		Visualisation(const std::string& simResultsFilepath_CSV, double KeyframeInterval_s, double simDuration_s);
		~Visualisation();

	private:
		void run();
		void init();
        void buildKeyframeHistory();
		void loadImGuiStyle();
		void handleInput(float frameTime_s);
		void update(float frameTime_s);
		void render();
		void handleTimeSelection(float frameTime_s);
        void saveAppState() const;
        void save(nlohmann::json& dest) const override;
        void loadAppState();
        void load(const std::string& source) override;

	};

}

#endif // F9SIM_GRAPHICS_VISUALISATION_H_
