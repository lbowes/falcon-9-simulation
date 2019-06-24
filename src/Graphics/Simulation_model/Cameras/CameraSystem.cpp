#include "CameraSystem.h"
#include "../../../Input/HWEventReceiver.h"

#include <IrrlichtDevice.h>
#include <ISceneManager.h>
#include <IMGUI/imgui.h>

namespace Graphics {

	CameraSystem::CameraSystem(irr::IrrlichtDevice& device, irr::scene::ISceneManager& sceneManager, Input::HWEventReceiver& input, float windowAspect) :
		mDevice(device),
		mSceneManager(sceneManager),
		mHWInput(input),
        mCurrentCamera(FPV),
        mHasFocus(false)
	{
		

        // --------------- CURRENT PROBLEM ----------------------
        // It is hard to get the position of the stage and the FPV camera's look-at vector to line up.
        // If we position the stage along the -Z axis for example, and set the camera's look-at vec to
        // point down this axis, you'd think that the camera would face the stage. This is not
        // the case - the camera points 90 degs clockwise (along the -X axis). Assuming that the mYaw 
        // value of the camera was off, we tried applying a simple offset of +90 degrees to fix this.

        // This fixed the problem for the X axis, but did not do anything to help the Z axis. The same
        // effect can be seen by subtracting 90 degs from mYaw (it works with Z but not X).
        

        mCameras.push_back(std::make_unique<FPVCamera>(
			sceneManager,
			input,
			chrono::Vector(0, 70.0, 0),       //-20.0, 3.0, 0.0 //-85.0, 30.0, 0.0
			irr::core::vector3df(0.0, -10.0, -1.0).normalize(), //1.0, 0.0, 0.0
			0.1f,
			10000.0f,
			windowAspect,
			44.7f
		)); 

		mCameras.push_back(std::make_unique<InterstageCamera>(sceneManager, windowAspect));

		sceneManager.setActiveCamera(&mCameras[FPV]->getInternalCamera());
		mDevice.getCursorControl()->setVisible(!mHasFocus);

		//mCameras.push_back(std::make_unique<ChaserCamera>(
		//	glm::vec3(1.0, 0.0, 0.0),
		//	0.1f,
		//	10000.0f,
		//	windowAspect,
		//	45.0f
		//));
	}

	void CameraSystem::update(chrono::ChCoordsys<double> stage1Transform_world, float windowAspect, float dt) {
		static_cast<FPVCamera*>(mCameras[FPV].get())->update(windowAspect, dt);
		static_cast<InterstageCamera*>(mCameras[interstage].get())->update(stage1Transform_world, windowAspect);
		//CHASER_CAM->update(windowAspect, stage1CoMPosition_world/* , dt */);
	}

	void CameraSystem::handleInput(irr::core::vector2di centreScreenPos_scr, float dt) {
		if(mDevice.isWindowFocused()) {
			if (mHWInput.getMouse().isButtonPressed(Input::MouseState::right) && mHasFocus) {
				mDevice.getCursorControl()->setPosition(centreScreenPos_scr);
				mDevice.getCursorControl()->setVisible(true);
				mHasFocus = false;
			}
			if (mHWInput.getMouse().isButtonPressed(Input::MouseState::left) && !ImGui::GetIO().WantCaptureMouse) {
				mDevice.getCursorControl()->setVisible(false);
				mHasFocus = true;
			}		

			if (mHasFocus && mDevice.isWindowActive()) { 
				ImGui::GetIO().ConfigFlags = ImGuiConfigFlags_NoMouse;
				mDevice.getCursorControl()->setPosition(centreScreenPos_scr);

				//Cycle through different cameras
				if(mHWInput.isKeyReleased(irr::KEY_KEY_C)) {
					//mCurrentCamera = mCurrentCamera + 1 > chaser ? FPV : mCurrentCamera + 1;
					mCurrentCamera = mCurrentCamera + 1 > interstage ? FPV : mCurrentCamera + 1;
					mSceneManager.setActiveCamera(&mCameras[mCurrentCamera]->getInternalCamera());
				}

				if(mCurrentCamera == FPV)
					static_cast<FPVCamera*>(mCameras[FPV].get())->handleInput(dt);
				//else if (mCurrentCamera == chaser)
					//CHASER_CAM->handleInput();
			}
			else
				ImGui::GetIO().ConfigFlags = ImGuiConfigFlags_None;
		}
		else {
			mDevice.getCursorControl()->setVisible(true);
			mHasFocus = false;
		}
	}

}