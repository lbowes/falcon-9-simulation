#include "AllCameras.h"
#include "../../../input/HWEventReceiver.h"

#include <core/ChQuaternion.h>
#include <core/ChVector.h>
#include <core/ChCoordsys.h>
#include <ISceneManager.h>
#include <ICameraSceneNode.h>
#include <IMGUI/imgui.h>

namespace Graphics {

	SimulationCamera::SimulationCamera(irr::scene::ISceneManager& sceneManager, chrono::ChVector<> position_world, irr::core::vector3df lookAtDir_world, float near, float far, float aspect, float FOVY_degs) :
        mPosition_world(position_world),
		mLookAtDir_world(lookAtDir_world)
	{
		mInternalCamera = sceneManager.addCameraSceneNode();

        mInternalCamera->setPosition({0, 0, 0});
		mInternalCamera->setUpVector({0, 1, 0});
        mInternalCamera->setTarget(lookAtDir_world);

        mInternalCamera->setNearValue(near);
		mInternalCamera->setFarValue(far);
		mInternalCamera->setAspectRatio(aspect);
		mInternalCamera->setFOV(irr::core::degToRad(FOVY_degs));
	}

	FPVCamera::FPVCamera(irr::scene::ISceneManager& sceneManager, Input::HWEventReceiver& input, float aspect) :
		SimulationCamera(sceneManager, chrono::Vector(0, 70.0, 0), irr::core::vector3df(0.0, -10.0, -1.0).normalize(), 0.1f, 300.0f, aspect, 44.7f),
		mMinMovementSpeed(6.4f),         // 25.0f
		mMaxMovementSpeed(20000.0f),     // 20000.0f
		mSpeedAdjustSensitivity(200.0f), // 200.0f
		mMovementFriction(7.0f),         // 7.0f
		mZoomSensitivity(0.1f),          // 0.1f
		mLookAroundSensitivity(0.05f),   // 0.05f
		mMovementSpeed(200.0f),          // 400.0f
		mPitch(-mLookAtDir_world.getSphericalCoordinateAngles().X), //mPitch(-lookAtDir_world.getHorizontalAngle().X),
		mYaw(mLookAtDir_world.getSphericalCoordinateAngles().Y),    //mYaw(lookAtDir_world.getHorizontalAngle().Y),
		mHWInput(input),
		mVelocity_world(0, 0, 0)
	{
        // TODO: Technically, the mPitch and mYaw initialisation above is still broken if we want to start the camera from
        // a certain state in code, but since state is saved and reloaded all the time, the app starts with the correct
        // set up. If we stop loading the saved state then these lines are not correct (they cause the camera to jump).

        clampPitchYaw();
        mLookAtDir_world = recalcLookAtVec(mPitch, mYaw);
    }

	void FPVCamera::update(float windowAspect, float dt) {
		mInternalCamera->setAspectRatio(windowAspect);

		mPosition_world += mVelocity_world * static_cast<double>(dt);
		mVelocity_world *= 1.0 / (1.0 + (dt * mMovementFriction));

        // Keep the camera above the ground at roughly eye-level
		//if(mPosition_world.y() < 1.74)
			//mPosition_world.y() = 1.74;

		mInternalCamera->setTarget(mLookAtDir_world);
	}

	void FPVCamera::handleInput(float dt) {
		handleDirectionInput(dt);
		handleZoomInput(dt);
		handleMovementInput(dt);
	}

    void FPVCamera::load(const std::string& source) {
        nlohmann::json j = nlohmann::json::parse(source);

        j["mMovementSpeed"].get_to(mMovementSpeed);
        j["mPitch"].get_to(mPitch);
        j["mYaw"].get_to(mYaw);

        j["mPosition_world"]["x"].get_to(mPosition_world.x());
        j["mPosition_world"]["y"].get_to(mPosition_world.y());
        j["mPosition_world"]["z"].get_to(mPosition_world.z());

        j["mVelocity_world"]["x"].get_to(mVelocity_world.x());
        j["mVelocity_world"]["y"].get_to(mVelocity_world.y());
        j["mVelocity_world"]["z"].get_to(mVelocity_world.z());

        j["mLookAtDir_world"]["x"].get_to(mLookAtDir_world.X);
        j["mLookAtDir_world"]["y"].get_to(mLookAtDir_world.Y);
        j["mLookAtDir_world"]["z"].get_to(mLookAtDir_world.Z);
    }

    void FPVCamera::save(nlohmann::json& dest) const {
        dest["mMovementSpeed"] = mMovementSpeed;
        dest["mPitch"] = mPitch;
        dest["mYaw"] = mYaw;

        // If the velocity is below a certain threshold, consider the camera to have to stopped moving
        // so next run, the camera begins from stationary state.
        chrono::ChVector<> vel = mVelocity_world;
        if(vel.Length() < 0.4)
            vel = 0;

        dest["mPosition_world"] = {{"x", mPosition_world.x()}, {"y", mPosition_world.y()}, {"z", mPosition_world.z()}};
        dest["mVelocity_world"] = {{"x", vel.x()}, {"y", vel.y()}, {"z", vel.z()}};
        dest["mLookAtDir_world"] = {{"x", mLookAtDir_world.X}, {"y", mLookAtDir_world.Y}, {"z", mLookAtDir_world.Z}};
    }

	void FPVCamera::handleDirectionInput(float dt) {
		using namespace irr::core;

		vector2di mouseDelta = mHWInput.getMouse().getDelta_scr();

		mYaw -= mouseDelta.X * mLookAroundSensitivity;
        mPitch -= mouseDelta.Y * mLookAroundSensitivity;

        // --------- TODO ------------
        // Consider creating a big debug screen for all the visualisation stuff, like camera properties (position/lookAt vectors, FOV etc.) Include everything
        // in one clean debug screen.

        clampPitchYaw();

		mLookAtDir_world = recalcLookAtVec(mPitch, mYaw);
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

    irr::core::vector3df FPVCamera::recalcLookAtVec(float pitch, float yaw) const {
        using namespace irr::core;

        vector3df result;

        result.X = cos(degToRad(pitch)) * cos(degToRad(yaw));
		result.Y = sin(degToRad(pitch));
		result.Z = cos(degToRad(pitch)) * sin(degToRad(yaw));
		result.normalize();

        return result;
    }

    void FPVCamera::clampPitchYaw() {
        mYaw -= 360.0f * floor(mYaw / 360.0f);
        mPitch = std::clamp(mPitch, -89.9f, 89.9f);
    }

	InterstageCamera::InterstageCamera(irr::scene::ISceneManager& sceneManager, float aspect) :
		mFOV(44.712002f),        //44.712002f
		SimulationCamera(sceneManager, chrono::Vector(0), irr::core::vector3df(0), 0.1f, 1000.0f, aspect, mFOV),
		mClockDegree_degs(45.0 + 10.882f), //45.0 + 10.882f, 45.0 to account for model rotation in blender
		mPitch_degs(2.222f),     //2.222f
		mHeight_stage(10.0f),    //45.89f
		mHeightAboveWall(1.951f) //1.951f
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

		mInternalCamera->setTarget({mLookAt_stage.x(), mLookAt_stage.y(), mLookAt_stage.z()});

		mPosition_world = mPosition_stage;
	}

	void InterstageCamera::update(chrono::ChCoordsys<double> stageTransform_world, float windowAspect) {
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
        // Transform the interstage camera's position into world space
        mPosition_world = stageTransform_world.TransformLocalToParent(mPosition_stage);
        //                ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

        // --------- CURRENTLY WORKING ON ------------
        // Look into line 209 above - why is this transformation causing the camera to bob up and down as it rotates with the stage?
        // If we just print stageTransform_world.pos, we see that the stage remains positioned at (0, 0, 0). So surely the transform
        // should not move the camera at all relative to the stage. It should just rotate exactly as the stage moves, but for some reason
        // the camera's vertical position changes

        ImGui::Begin("Interstage cam info");
		ImGui::Text("abs pos: %.3f, %.3f, %.3f\n", mPosition_world.x(), mPosition_world.y(), mPosition_world.z());
		ImGui::End();

        // Transform its direction (look at) vector into world space
        chrono::Vector lookAt_world = stageTransform_world.TransformDirectionLocalToParent(mLookAt_stage);
        mInternalCamera->setTarget(irr::core::vector3df(float(lookAt_world.x()), float(lookAt_world.y()), float(lookAt_world.z())));

        chrono::Vector up_world = stageTransform_world.TransformDirectionLocalToParent(mUp_stage);
        mInternalCamera->setUpVector(irr::core::vector3df(float(up_world.x()), float(up_world.y()), float(up_world.z())));
#endif

		mInternalCamera->setAspectRatio(windowAspect);
	}

}