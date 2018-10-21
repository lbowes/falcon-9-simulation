/* The position of SimulationCamera will always be fixed at (0, 0, 0). It will *always* be on the origin.
 * The direction of this camera (including the front and up vectors) will change, as this does not have
 * any effect on precision. 
*/

#ifndef GRAPHICS_ALLCAMERAS_H
#define GRAPHICS_ALLCAMERAS_H
#pragma once

#include "PhysicsFramework/CoordTransform3D.h"

#include <GraphicsFramework/PerspectiveCamera.h>
#include <GraphicsFramework/Input.h>
#include <PhysicsFramework/State.h>
#include <glm/gtx/vector_angle.hpp>

#define CONFIGURE_INTERSTAGE_CAM 0

namespace Graphics {

	class SimulationCamera {
	protected:
		glm::dvec3 mPosition;
		GF::PerspectiveCamera mPerspectiveCamera;

	public:
		SimulationCamera(float near, float far, float aspect, float FOV);
		~SimulationCamera() = default;

		glm::mat4 getViewProjection_generated() const;
		const GF::Camera& getInternalCamera() const { return mPerspectiveCamera; }
		GF::Camera& getInternalCamera() { return mPerspectiveCamera; }

		const glm::dvec3& getPosition() const { return mPosition; }

	};

	class FPVCamera : public SimulationCamera {
	private:
		const float
			mMinMovementSpeed = 25.0f,        //10.0f
			mMaxMovementSpeed = 20000.0f,     //20000.0f
			mSpeedAdjustSensitivity = 200.0f, //200.0f
			mMovementFriction = 7.0f,         //7.0f
			mZoomSensitivity = 0.1f,          //0.1f
			mLookAroundSensitivity = 0.05f;   //0.05f

		float
			mMovementSpeed = 400.0f,          //400.0f  
			mFOV = 45.0f,
			mPitch = 0.0f,
			mYaw = 0.0f;

		glm::dvec3 mVelocity;
		
		glm::vec3 mDirection;

	public:
		FPVCamera(glm::dvec3 position, glm::vec3 direction, float near, float far, float aspect, float FOV);
		~FPVCamera() = default;

		void update(float windowAspect, float dt);
		void handleInput(float dt);

	private:
		void handleDirectionInput();
		void handleMovementInput(float dt);

	};

	class InterstageCamera : public SimulationCamera {
	private:
		const float
			mClockDegree_degs = 10.882f,
			mPitch_degs = 2.222f,
			mHeight_stage = 45.89f,
			mHeightAboveWall = 1.951f,
			mFOV = 44.712002f;

		glm::vec3
			mPosition_stage,
			mFront_stage,
			mUp_stage;

	public:
		InterstageCamera(float aspect);
		~InterstageCamera() = default;

		void update(const CoordTransform3D& stage1ToWorld, float windowAspect/* , float dt */);

	};

	class ChaserCamera : public SimulationCamera {
	private:
		const glm::vec3 mPosition_stage = glm::vec3(-0.401277f, 44.73737f, -1.850528f);

		const float
			mZoomSensitivity = 0.1f,        //0.1f
			mLookAroundSensitivity = 0.05f; //0.05f

		glm::vec3 mDirection;

		float
			mPitch = 0.0f,
			mYaw = 0.0f;

	public:
		ChaserCamera(glm::vec3 direction, float near, float far, float aspect, float FOV);
		~ChaserCamera() = default;

		void update(float windowAspect, glm::dvec3 stage1CoMPosition_world/* , float dt */);
		void handleInput();

	};

}

#endif
