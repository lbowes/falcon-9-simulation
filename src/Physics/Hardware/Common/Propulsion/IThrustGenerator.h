#ifndef PHYSICS_HARDWARE_THRUSTGENERATOR_H
#define PHYSICS_HARDWARE_THRUSTGENERATOR_H
#pragma once

#include "Physics/Hardware/Common/Core/IStageComponent.hpp"

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Physics {
	namespace Hardware {

		class IThrustGenerator : public IStageComponent {
			friend class ThrustGeneratorGroup;
		protected:
			glm::dvec3 mThrust_stage;			//thrust produced in stage space
			bool mActive = false;	    
			double mThrustMagnitude = 0.0; //N

		public:
			IThrustGenerator(CoordTransform3D thrusterToStage);
			virtual ~IThrustGenerator() = default;

			void update(double dt);

			glm::dvec3 getThrust_stage() const { return mThrust_stage; }
			bool isActive() const { return mActive; }
			void setActive(bool newState) { mActive = newState; }

		private:
			//Updates any custom features that individual devices have. 
			//E.g. (for a renderable gas thruster) mIsVisible, or chamber pressure for an Engine etc
			virtual void updateDeviceSpecific(double dt) = 0;

		};

	}
}

#endif
