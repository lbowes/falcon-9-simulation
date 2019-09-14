#include "SimulationModel.h"
#include "../../input/HWEventReceiver.h"
#include "../../physics/external/GroundPlane.h"

#include <ISceneManager.h>
#include <IVideoDriver.h>

namespace Graphics {

	SimulationModel::SimulationModel(irr::video::IVideoDriver& vidDriver, irr::scene::ISceneManager& sceneManager) :
		mSceneManager(sceneManager),
        mLightSystem(sceneManager),
        mFalcon9Model(sceneManager),
		mGroundModel(vidDriver)
	{ }

	void SimulationModel::update(const chrono::Vector& currentCamPos_world, const Falcon9Keyframe& Keyframe) {
		mFalcon9Model.update(currentCamPos_world, Keyframe);
		mLightSystem.update(currentCamPos_world);
	}

	void SimulationModel::render(const chrono::Vector& currentCamPos_world) {
		mSceneManager.drawAll();
		mGroundModel.render(currentCamPos_world);
	}

}