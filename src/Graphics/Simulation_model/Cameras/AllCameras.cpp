#include "AllCameras.h"
#include "../../../Input/HWEventReceiver.h"
#include <ISceneManager.h>
#include <ICameraSceneNode.h>
#include <chrono/core/ChQuaternion.h>
#include <IMGUI/imgui.h>

namespace Graphics {

	SimulationCamera::SimulationCamera(irr::scene::ISceneManager& sceneManager, chrono::ChVector<> position_world, irr::core::vector3df lookAtDir_world, float near, float far, float aspect, float FOVY_degs) :
		mPosition_world(position_world),
		mLookAtDir_world(lookAtDir_world)
	{
		mInternalCamera = sceneManager.addCameraSceneNode();
		mInternalCamera->setNearValue(near);
		mInternalCamera->setUpVector({0, 1, 0});
		
		irr::core::vector3df targetPos = irr::core::vector3df(position_world.x(), position_world.y(), position_world.z()) + lookAtDir_world;
		mInternalCamera->setTarget(targetPos);
		mInternalCamera->setFarValue(far);
		mInternalCamera->setAspectRatio(aspect);
		mInternalCamera->setFOV(FOVY_degs * chrono::CH_C_DEG_TO_RAD);
		mInternalCamera->bindTargetAndRotation(true);
	}

	FPVCamera::FPVCamera(irr::scene::ISceneManager& sceneManager, Input::HWEventReceiver& input, chrono::ChVector<> position_world, irr::core::vector3df lookAtDir_world, float near, float far, float aspect, float FOVY_degs) :
		SimulationCamera(sceneManager, position_world, lookAtDir_world, near, far, aspect, FOVY_degs),
		mMinMovementSpeed(6.4f),        //25.0f
		mMaxMovementSpeed(20000.0f),     //20000.0f
		mSpeedAdjustSensitivity(200.0f), //200.0f
		mMovementFriction(7.0f),         //7.0f
		mZoomSensitivity(0.1f),          //0.1f
		mLookAroundSensitivity(0.05f),   //0.05f
		mMovementSpeed(200.0f),          //400.0f  
		mPitch(0.0f),
		mYaw(0.0f),
		mHWInput(input),
		mVelocity_world(0, 0, 0)
	{ 
		mInternalCamera->setPosition({0, 0, 0});
	}

	void FPVCamera::update(float windowAspect, float dt) {
		mInternalCamera->setAspectRatio(windowAspect);

		mPosition_world += mVelocity_world * static_cast<double>(dt);
		mVelocity_world *= 1.0 / (1.0 + (dt * mMovementFriction));

		if(mPosition_world.y() < 1.76)
			mPosition_world.y() = 1.76;

		mInternalCamera->setTarget(mLookAtDir_world);
	}

	void FPVCamera::handleInput(float dt) {
		handleDirectionInput(dt);
		handleZoomInput(dt);
		handleMovementInput(dt);
	}

	void FPVCamera::handleDirectionInput(float dt) {
		using namespace irr::core;

		vector2di mouseDelta = mHWInput.getMouse().getDelta_scr();

		mYaw -= mouseDelta.X * mLookAroundSensitivity;
		mPitch -= mouseDelta.Y * mLookAroundSensitivity;

		if (mPitch > 89.0f) 
			mPitch = 89.0f;
		if (mPitch < -89.0f) 
			mPitch = -89.0f;

		mLookAtDir_world.X = cos(degToRad(mPitch)) * cos(degToRad(mYaw));
		mLookAtDir_world.Y = sin(degToRad(mPitch));
		mLookAtDir_world.Z = cos(degToRad(mPitch)) * sin(degToRad(mYaw));
		mLookAtDir_world.normalize();
	}

	void FPVCamera::handleMovementInput(float dt) {
		using namespace irr;
		using namespace irr::core;

		mLookAtDir_world.normalize();

		//Movement
		if (mHWInput.isKeyPressed(KEY_KEY_W)) {
			vector3df dir = vector3df(mLookAtDir_world.X, 0.0f, mLookAtDir_world.Z).normalize();
			mVelocity_world += chrono::ChVector(dir.X, dir.Y, dir.Z) * mMovementSpeed * dt;
		}
		
		if (mHWInput.isKeyPressed(KEY_KEY_S)) {
			vector3df dir = vector3df(mLookAtDir_world.X, 0.0f, mLookAtDir_world.Z).normalize();
			mVelocity_world -= chrono::ChVector(dir.X, dir.Y, dir.Z) * mMovementSpeed * dt;
		}         
		
		if (mHWInput.isKeyPressed(KEY_KEY_A)) {
			vector3df dir = vector3df(mLookAtDir_world.X, 0.0f, mLookAtDir_world.Z).normalize().crossProduct({0, 1, 0});
			mVelocity_world += chrono::ChVector(dir.X, dir.Y, dir.Z) * mMovementSpeed * dt;
		}
		
		if (mHWInput.isKeyPressed(KEY_KEY_D)) {
			vector3df dir = vector3df(mLookAtDir_world.X, 0.0f, mLookAtDir_world.Z).normalize().crossProduct({0, 1, 0});
			mVelocity_world -= chrono::ChVector(dir.X, dir.Y, dir.Z) * mMovementSpeed * dt;
		}
		
		if (mHWInput.isKeyPressed(KEY_SPACE))
			mVelocity_world.y() += mMovementSpeed * dt;
		
		if (mHWInput.isKeyPressed(KEY_LSHIFT))
			mVelocity_world.y() -= mMovementSpeed * dt;
	}

	void FPVCamera::handleZoomInput(float dt) {
		if(ImGui::GetIO().WantCaptureMouse)
			return;

		//Movement speed
		float mouseScroll = mHWInput.getMouse().getScroll();
		mMovementSpeed += mouseScroll * mSpeedAdjustSensitivity;
		mMovementSpeed = std::clamp(mMovementSpeed, mMinMovementSpeed, mMaxMovementSpeed);
	}

	InterstageCamera::InterstageCamera(irr::scene::ISceneManager& sceneManager, float aspect) :
		mClockDegree_degs(45.0 + 10.882f), //45.0 + 10.882f, 45.0 to account for model rotation in blender
		mPitch_degs(2.222f),        //2.222f
		mHeight_stage(45.89f),      //45.89f
		mHeightAboveWall(1.951f),   //1.951f
		mFOV(44.712002f),           //44.712002f
		SimulationCamera(sceneManager, chrono::Vector(0), irr::core::vector3df(0), 0.1f, 1000.0f, aspect, mFOV)
	{
		using namespace chrono;

		//Rotate the camera's position into place around the stage
		Quaternion clockingRotation = Q_from_AngY(-mClockDegree_degs * CH_C_DEG_TO_RAD);
		clockingRotation.Normalize();
		Vector pos_stage2D = Vector(0, mHeight_stage, mHeightAboveWall);
		mPosition_stage = clockingRotation.Rotate(pos_stage2D);

		//Rotate the camera's up vector into place around the stage
		Quaternion pitchRotation = Q_from_AngAxis(-mPitch_degs * CH_C_DEG_TO_RAD, VECT_X);
		mUp_stage = pitchRotation.Rotate(VECT_Z);
		mUp_stage = clockingRotation.Rotate(mUp_stage);

		//Rotate the camera's front vector into place around the stage
		mLookAt_stage = pitchRotation.Rotate(-VECT_Y);
		mLookAt_stage = clockingRotation.Rotate(mLookAt_stage);

		mInternalCamera->setFOV(mFOV);
		mInternalCamera->setAspectRatio(aspect);
		mInternalCamera->setTarget({mLookAt_stage.x(), mLookAt_stage.y(), mLookAt_stage.z()});
		
		mPosition_world = mPosition_stage;
	}

	void InterstageCamera::update(float windowAspect) {
		//using namespace glm;

#if CONFIGURE_INTERSTAGE_CAM
		static float
			clockDegree_degs = mClockDegree_degs,
			pitch_degs = mPitch_degs,
			height_stage = mHeight_stage,
			heightAboveWall = mHeightAboveWall,
			FOV = mFOV;
		
		ImGui::SliderFloat("Angle around stage", &clockDegree_degs, 0.0f, 360.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset angle"))
			clockDegree_degs = mClockDegree_degs;
		
		ImGui::SliderFloat("Angle up", &pitch_degs, 0.0f, 20.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset angle up"))
			pitch_degs = mPitch_degs;

		ImGui::SliderFloat("Height on stage", &height_stage, 44.5f, 47.5f);
		ImGui::SameLine();
		if (ImGui::Button("Reset height"))
			height_stage = mHeight_stage;

		ImGui::SliderFloat("Radial distance", &heightAboveWall, 1.88f, 1.98f);
		ImGui::SameLine();
		if (ImGui::Button("Reset radial distance"))
			heightAboveWall = mHeightAboveWall;

		ImGui::SliderFloat("FOV", &FOV, 44.0f, 46.0f);
		ImGui::SameLine();
		if (ImGui::Button("Reset FOV"))
			FOV = mFOV;

		glm::vec3
			position_stage = glm::rotate(glm::vec3(0.0f, height_stage, heightAboveWall), glm::radians(clockDegree_degs), { 0.0f, 1.0f, 0.0f }),
			up_stage = glm::rotate(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(clockDegree_degs), { 0.0f, 1.0f, 0.0f }),
			front_stage = glm::rotate(glm::vec3(0.0f, -1.0f, 0.0f), glm::radians(pitch_degs), glm::cross(normalize(up_stage), glm::vec3(0.0f, 1.0f, 0.0f)));

		up_stage = glm::rotate(up_stage, glm::radians(pitch_degs), glm::cross(normalize(up_stage), glm::vec3(0.0f, 1.0f, 0.0f)));

		mPosition = mStage1State.getObjectSpace().toParentSpace(position_stage);
		mPerspectiveCamera.setFront(mStage1State.getObjectSpace().toParentSpace_rotation(front_stage));
		mPerspectiveCamera.setUp(mStage1State.getObjectSpace().toParentSpace_rotation(up_stage));
		mPerspectiveCamera.setFOVY(FOV);
		
		printf("clockDegree_degs: %f\npitch_degs: %f\nheight_stage: %f\nheightAboveWall: %f\nFOV: %f\n\n\n", clockDegree_degs, pitch_degs, height_stage, heightAboveWall, FOV);
#else
		//mPosition = stage1ToWorld.toParentSpace(mPosition_stage);
		//mPerspectiveCamera.setFront(stage1ToWorld.toParentSpace_rotation(mFront_stage));
		//mPerspectiveCamera.setUp(stage1ToWorld.toParentSpace_rotation(mUp_stage));
#endif

		mInternalCamera->setAspectRatio(windowAspect);
	}

}