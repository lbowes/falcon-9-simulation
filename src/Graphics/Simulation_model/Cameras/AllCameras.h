#ifndef GRAPHICS_ALLCAMERAS_H
#define GRAPHICS_ALLCAMERAS_H
#pragma once

#include <core/ChVector.h>
#include <vector3d.h>

namespace Input {
	class HWEventReceiver;
}

namespace irr {
	namespace scene {
		class ISceneManager;
		class ICameraSceneNode;
	}
}

namespace Graphics {

	/*
	- A camera with a high precision position.
	- In terms of OpenGL, the mInternalCamera's position remains fixed at the origin.
	*/
	class SimulationCamera {
	protected:
		chrono::Vector mPosition_world; //High precision
		
		irr::core::vector3df mLookAtDir_world; //Low precision
		
		irr::scene::ICameraSceneNode* mInternalCamera;

	public:
		SimulationCamera(
			irr::scene::ISceneManager& sceneManager, 
			chrono::ChVector<> position_world = {0, 0, 0},
			irr::core::vector3df lookAtDir_world = {0, 0, -1}, 
			float near = 0.1f, 
			float far = 1000.0f, 
			float aspect = 1920.0f / 1080.0f, 
			float FOVY_degs = 45.0f
		);

		virtual ~SimulationCamera() = default;

		void setPosition_world(chrono::Vector newPosition_world) { mPosition_world = newPosition_world; }
		chrono::Vector getPosition_world() const { return mPosition_world; }
		irr::scene::ICameraSceneNode& getInternalCamera() { return *mInternalCamera; }
		const irr::scene::ICameraSceneNode& getInternalCamera() const { return *mInternalCamera; }

	};

	class FPVCamera : public SimulationCamera {
	private:
		const float
			mMinMovementSpeed,
			mMaxMovementSpeed,
			mSpeedAdjustSensitivity,
			mMovementFriction,
			mZoomSensitivity,
			mLookAroundSensitivity;

		float
			mMovementSpeed, 
			mPitch,
			mYaw;

		Input::HWEventReceiver& mHWInput;
		
		chrono::ChVector<> mVelocity_world;

	public:
		FPVCamera(
			irr::scene::ISceneManager& sceneManager, 
			Input::HWEventReceiver& input, 
			chrono::ChVector<> position_world = {0, 0, 0}, 
			irr::core::vector3df lookAtDir_world = {0, 0, -1}, 
			float near = 0.1f, 
			float far = 1000.0f, 
			float aspect = 1920.0f / 1080.0f, 
			float FOVY_degs = 45.0f
		);

		~FPVCamera() = default;

		void update(float windowAspect, float dt);
		void handleInput(float dt);

	private:
		void handleDirectionInput(float dt);
		void handleMovementInput(float dt);
		void handleZoomInput(float dt);

	};

	class InterstageCamera : public SimulationCamera {
	private:
		const float
			mClockDegree_degs,
			mPitch_degs,
			mHeight_stage,
			mHeightAboveWall,
			mFOV;

		chrono::ChVector<float>
			mPosition_stage,
			mLookAt_stage,
			mUp_stage;

	public:
		InterstageCamera(irr::scene::ISceneManager& sceneManager, float aspect);
		~InterstageCamera() = default;

		void update(float windowAspect);

	};

}

#endif
