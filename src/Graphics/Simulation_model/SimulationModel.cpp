#include "SimulationModel.h"
#include "../../Input/HWEventReceiver.h"
#include "../../Physics/External/GroundPlane.h"

#include <ISceneManager.h>
#include <IVideoDriver.h>

namespace Graphics {

	SimulationModel::SimulationModel(irr::video::IVideoDriver& vidDriver, irr::scene::ISceneManager& sceneManager) :
		mSceneManager(sceneManager),
        mLightSystem(sceneManager),
        mFalcon9Model(sceneManager),
		mGroundModel(vidDriver)
	{ }

	void SimulationModel::update(const chrono::Vector& currentCamPos_world, const ModelKeyFrame& keyFrame) {
		mFalcon9Model.update(currentCamPos_world, keyFrame);
		mLightSystem.update(currentCamPos_world);
	}

	void SimulationModel::render(const chrono::Vector& currentCamPos_world) {
		mSceneManager.drawAll();
		mGroundModel.render(currentCamPos_world);
	}

}