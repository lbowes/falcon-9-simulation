#ifndef GRAPHICS_VISUALISATION_H
#define GRAPHICS_VISUALISATION_H
#pragma once

#include "../Input/MasterEventReceiver.h"
#include "../Input/HWEventReceiver.h"
#include "GUI/GUILayer.h"
#include "Simulation_model/Cameras/CameraSystem.h"
#include "Simulation_model/SimulationModelLayer.h"
#include "../Physics/Internal/Hardware/Falcon_9/F9.h"

#include <IrrIMGUI/IrrIMGUI.h>
#include <map>

//temp
#include <physics/ChBodyEasy.h>
//


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
	};

	class Visualisation {
	private:
		irr::IrrlichtDevice* mDevice;
		irr::video::IVideoDriver* mVidDriver;
    	irr::scene::ISceneManager* mSceneMgr;
	    IrrIMGUI::IIMGUIHandle* mImGuiHandle;

		Input::HWEventReceiver mHWinput;
		Input::MasterEventReceiver mEventReceiver;
		IrrIMGUI::CIMGUIEventReceiver mImGuiEventReceiver;

		std::unique_ptr<CameraSystem> mCameraSystem;
		std::unique_ptr<SimulationModelLayer> mModelLayer;
		std::unique_ptr<GUI::GUILayer> mGUILayer;

		const std::map<const unsigned, const Physics::F9_DSS>& mStateHistory;

		const double 
			mSnapshotInterval_s,
			mSimDuration;

		PlaybackConfig mPlayback;

		//temp
		Physics::F9_DSS mLiveSnapshot_temp;
		//

	public:
		Visualisation(const std::map<const unsigned, const Physics::F9_DSS>& stateHistoryHandle, double snapshotInterval_s, double simDuration);
		~Visualisation();

		void run();

	private:
		void init();
		void loadImGuiStyle();
		void handleInput(float frameTime_s);
		void update(float frameTime_s);
		void render();
		void handleTimeSelection(float frameTime_s);

	};

}

#endif