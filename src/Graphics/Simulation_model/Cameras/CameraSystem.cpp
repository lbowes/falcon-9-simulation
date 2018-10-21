#include "CameraSystem.h"

namespace Graphics {

	CameraSystem::CameraSystem(float windowAspect) {
		addAllCameras(windowAspect);
	}

	void CameraSystem::update(const CoordTransform3D& stage1ToWorld, float windowAspect, float dt, glm::dvec3 stage1CoMPosition_world) {
		FPV_CAM->update(windowAspect, dt);
		INTERSTAGE_CAM->update(stage1ToWorld, windowAspect/* , dt */);
		CHASER_CAM->update(windowAspect, stage1CoMPosition_world/* , dt */);
	}

	void CameraSystem::checkInput(float dt) {
		//Cycle through different cameras
		if (GF::Input::isKeyReleased(GLFW_KEY_C)) 
			mCurrentCamera = mCurrentCamera + 1 > chaser ? FPV : mCurrentCamera + 1;
		
		if (!mHasFocus) 
			return;
		
		if(mCurrentCamera == FPV)
			FPV_CAM->handleInput(dt);
		else if (mCurrentCamera == chaser)
			CHASER_CAM->handleInput();
	}

	void CameraSystem::addAllCameras(float windowAspect) {
		//temp
		//Physics::External::SurfaceLocation temp({ "temp", glm::dvec3(53.826687, -2.419171, 0.0) });
		//glm::dvec3 
		//	pos_eun = glm::dvec3(temp.getEUN_to_ECEFTransform().getLocalToParent_rotation() * glm::dvec4(20.0, 5.0, 0.0, 1.0)),
		//	front_eun = glm::dvec3(temp.getEUN_to_ECEFTransform().getLocalToParent_rotation() * glm::dvec4(-1.0, 0.0, 0.0, 1.0));
		//

		mCameras.push_back(std::make_unique<FPVCamera>(
			glm::dvec3(-85.0, 30.0, 0.0),
			glm::dvec3(1.0, 0.0, 0.0),
			0.1f,
			10000.0f,
			windowAspect,
			45.0f));

		mCameras.push_back(std::make_unique<InterstageCamera>(windowAspect));

		mCameras.push_back(std::make_unique<ChaserCamera>(
			glm::vec3(1.0, 0.0, 0.0),
			0.1f,
			10000.0f,
			windowAspect,
			45.0f));
	}

}