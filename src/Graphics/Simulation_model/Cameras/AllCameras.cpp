#include "AllCameras.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Falcon9Stage1.h"

#if CONFIGURE_INTERSTAGE_CAM
#include <GraphicsFramework/Vendor/ImGui/imgui.h>
#endif

namespace Graphics {

	SimulationCamera::SimulationCamera(float near, float far, float aspect, float FOV) :
		mPerspectiveCamera(glm::vec3(), glm::vec3(), glm::vec3(), near, far, aspect, FOV)
	{ }

	glm::mat4 SimulationCamera::getViewProjection_generated() const 
		//In order to minimise floating point errors, the OpenGL positions of the internal cameras in SimulationCamera objects
		//DO NOT CHANGE. The position remains fixed at the origin, while the direction changes. Models use the mGraphicsState 
		//to calculate their own transforms relative to the camera to give the effect that the camera is moving (when really 
		//it is fixed at (0, 0, 0).
		//However, this means that the view matrix of the camera does not have any positional component to it. So even though
		//the SimulationCamera *appears* to move, the internal camera's view matrix DOES NOT REFLECT THIS as it only accounts
		//for direction.
		//The function below has been added to generate a correct view projection to reflect the camera's movement. This is
		//used in areas where a correct view matrix is required, but precision is not an issue (2D marker positioning).
		//It should not be used to render simulation objects as normal, as this would defeat the whole purpose of eliminating
		//precision errors.
	{
		return 
			mPerspectiveCamera.getProjection() * 
			glm::lookAt(glm::vec3(mCameraState.mPosition_highP), glm::vec3(mCameraState.mPosition_highP) + mPerspectiveCamera.getFront(), mPerspectiveCamera.getUp());
	}

	FPVCamera::FPVCamera(glm::dvec3 position_highP, glm::vec3 direction_OGL, float near, float far, float aspect, float FOV) :
		SimulationCamera(near, far, aspect, FOV),
		mDirection_OGL(direction_OGL)
	{
		using namespace glm;

		mPerspectiveCamera.setFront(direction_OGL);
		mPerspectiveCamera.setUp({ 0.0f, 1.0f, 0.0f });

		//Calculating intial yaw and pitch values from direction vector
		vec3 horizontalDirection = normalize(vec3(direction_OGL.x, 0.0f, direction_OGL.z));
		mYaw = degrees(orientedAngle(horizontalDirection, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
		mPitch = degrees(asin(normalize(direction_OGL).y));

		mCameraState.mPosition_highP = position_highP;
	}

	void FPVCamera::update(float windowAspect, float dt) {
		mPerspectiveCamera.setAspect(windowAspect);

		mCameraState.mPosition_highP += mVelocity_highP * static_cast<double>(dt);
		mVelocity_highP *= 1.0 / (1.0 + (dt * mMovementFriction));
	}

	void FPVCamera::handleInput(float dt) {
		handleDirectionInput();
		handleMovementInput(dt);
	}

	void FPVCamera::handleDirectionInput() {
		glm::vec2 mouseDelta = GF::Input::getMouseDelta();
		
		mYaw += mouseDelta.x * mLookAroundSensitivity;
		mPitch -= mouseDelta.y * mLookAroundSensitivity;

		if (mPitch > 89.0f) mPitch = 89.0f;
		if (mPitch < -89.0f) mPitch = -89.0f;

		mDirection_OGL.x = cos(glm::radians(mPitch)) * cos(glm::radians(mYaw));
		mDirection_OGL.y = sin(glm::radians(mPitch));
		mDirection_OGL.z = cos(glm::radians(mPitch)) * sin(glm::radians(mYaw));
		mDirection_OGL = normalize(mDirection_OGL);

		mCameraState.mOrientation_highP = glm::angleAxis(0.0, glm::dvec3(mDirection_OGL));
		mPerspectiveCamera.setFront(mDirection_OGL);
	}

	void FPVCamera::handleMovementInput(float dt) {
		using namespace GF;
		using namespace glm;

		if (Input::isKeyPressed(GLFW_KEY_W))
			mVelocity_highP += normalize(vec3(mDirection_OGL.x, 0.0f, mDirection_OGL.z)) * mMovementSpeed * dt;
		
		if (Input::isKeyPressed(GLFW_KEY_S))          
			mVelocity_highP -= normalize(vec3(mDirection_OGL.x, 0.0f, mDirection_OGL.z)) * mMovementSpeed * dt;
		
		if (Input::isKeyPressed(GLFW_KEY_A))          
			mVelocity_highP -= normalize(cross(mDirection_OGL, vec3(0.0f, 1.0f, 0.0f))) * mMovementSpeed * dt;
		
		if (Input::isKeyPressed(GLFW_KEY_D))          
			mVelocity_highP += normalize(cross(mDirection_OGL, vec3(0.0f, 1.0f, 0.0f))) * mMovementSpeed * dt;
		
		if (Input::isKeyPressed(GLFW_KEY_SPACE))      
			mVelocity_highP.y += mMovementSpeed * dt;
		
		if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT)) 
			mVelocity_highP.y -= mMovementSpeed * dt;

		//Speed adjustment
		float mouseScroll = GF::Input::getMouseScroll();
		mMovementSpeed += mouseScroll * mSpeedAdjustSensitivity;

		if(mMovementSpeed > mMaxMovementSpeed)
			mMovementSpeed = mMaxMovementSpeed;

		if(mMovementSpeed < mMinMovementSpeed)
			mMovementSpeed = mMinMovementSpeed;

		if (Input::isMouseButtonReleased(GLFW_MOUSE_BUTTON_MIDDLE) || Input::isMouseButtonReleased(GLFW_MOUSE_BUTTON_4))
			mMovementSpeed = 400.0f;
	}

	InterstageCamera::InterstageCamera(const State& stage1State, float aspect) :
		SimulationCamera(0.01f, 1000.0f, aspect, mFOV),
		mStage1State(stage1State)
	{ 
		mPosition_stage = glm::rotate(glm::vec3(0.0f, mHeight_stage, mHeightAboveWall), glm::radians(mClockDegree_degs), { 0.0f, 1.0f, 0.0f });
		mUp_stage = glm::rotate(glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(mClockDegree_degs), { 0.0f, 1.0f, 0.0f });
		mUp_stage = glm::rotate(mUp_stage, glm::radians(mPitch_degs), glm::cross(normalize(mUp_stage), glm::vec3(0.0f, 1.0f, 0.0f)));
		mFront_stage = glm::rotate(glm::vec3(0.0f, -1.0f, 0.0f), glm::radians(mPitch_degs), glm::cross(normalize(mUp_stage), glm::vec3(0.0f, 1.0f, 0.0f)));

		mCameraState.mPosition_highP = stage1State.getObjectSpace().toParentSpace(mPosition_stage);
		mPerspectiveCamera.setFront(stage1State.getObjectSpace().toParentSpace_rotation(mFront_stage));
		mPerspectiveCamera.setUp(stage1State.getObjectSpace().toParentSpace_rotation(mUp_stage));
		mPerspectiveCamera.setFOVY(mFOV);
		mPerspectiveCamera.setAspect(aspect);
	}

	void InterstageCamera::update(float windowAspect/* , float dt */) {
		using namespace glm;

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

		mCameraState.mPosition_highP = mStage1State.getObjectSpace().toParentSpace(position_stage);
		mPerspectiveCamera.setFront(mStage1State.getObjectSpace().toParentSpace_rotation(front_stage));
		mPerspectiveCamera.setUp(mStage1State.getObjectSpace().toParentSpace_rotation(up_stage));
		mPerspectiveCamera.setFOVY(FOV);
		
		printf("clockDegree_degs: %f\npitch_degs: %f\nheight_stage: %f\nheightAboveWall: %f\nFOV: %f\n\n\n", clockDegree_degs, pitch_degs, height_stage, heightAboveWall, FOV);
#else
		mCameraState.mPosition_highP = mStage1State.getObjectSpace().toParentSpace(mPosition_stage);
		mPerspectiveCamera.setFront(mStage1State.getObjectSpace().toParentSpace_rotation(mFront_stage));
		mPerspectiveCamera.setUp(mStage1State.getObjectSpace().toParentSpace_rotation(mUp_stage));
#endif

		mPerspectiveCamera.setAspect(windowAspect);
	}

	ChaserCamera::ChaserCamera(glm::vec3 direction_OGL, float near, float far, float aspect, float FOV) :
		SimulationCamera(near, far, aspect, FOV)
	{
		using namespace glm;

		mPerspectiveCamera.setFront(direction_OGL);
		mPerspectiveCamera.setUp({ 0.0f, 1.0f, 0.0f });

		//Calculating intial yaw and pitch values from direction vector
		vec3 horizontalDirection = normalize(vec3(direction_OGL.x, 0.0f, direction_OGL.z));
		mYaw = degrees(orientedAngle(horizontalDirection, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
		mPitch = degrees(asin(normalize(direction_OGL).y));
	}

	void ChaserCamera::update(float windowAspect, glm::dvec3 stage1CoMPosition_world/* , float dt */) {
		mPerspectiveCamera.setAspect(windowAspect);

		mCameraState.mPosition_highP = stage1CoMPosition_world - glm::dvec3(100.0f, 0.0f, 0.0f);
	}
	
	void ChaserCamera::handleInput() {
		glm::vec2 mouseDelta = GF::Input::getMouseDelta();

		mYaw += mouseDelta.x * mLookAroundSensitivity;
		mPitch -= mouseDelta.y * mLookAroundSensitivity;

		if (mPitch > 89.0f) mPitch = 89.0f;
		if (mPitch < -89.0f) mPitch = -89.0f;

		mDirection_OGL.x = cos(glm::radians(mPitch)) * cos(glm::radians(mYaw));
		mDirection_OGL.y = sin(glm::radians(mPitch));
		mDirection_OGL.z = cos(glm::radians(mPitch)) * sin(glm::radians(mYaw));
		mDirection_OGL = normalize(mDirection_OGL);

		mCameraState.mOrientation_highP = glm::angleAxis(0.0, glm::dvec3(mDirection_OGL));
		mPerspectiveCamera.setFront(mDirection_OGL);
	}

}