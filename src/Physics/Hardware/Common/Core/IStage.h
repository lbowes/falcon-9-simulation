#ifndef PHYSICS_HARDWARE_ISTAGE_H
#define PHYSICS_HARDWARE_ISTAGE_H
#pragma once

#include "Physics/Hardware/Common/Propulsion/ThrustGeneratorGroup.h"
#include "Physics/Hardware/Common/Propulsion/GasThruster.h"
#include "Physics/Hardware/Common/Propellant/FluidTankGroup.h"
#include "Physics/External/Environment.h"
#include "definitions.h"

#include <PhysicsFramework/RigidBodyGroup.h>

namespace Physics {
	namespace Hardware {
		
		class IStage : public RigidBodyGroup {
		protected:
			//TODO:
			//Physics::AerodynamicModel mAeroModel;
			
			/*Potential approach to consider if the stage object is inherited from and contains literally nothing - if it is *all* added by
			the programmer.
			std::vector<FuelSupply*> mFuelSupplies;
			std::vector<IThrustGeneratorGroup*> mPropDeviceGroups;
			foo.addFuelSupply(bar); etc */

			ThrustGeneratorGroup
				mThrusters,
				mEngines;

			FluidTankGroup mPropellantSupplies;

			double
				mMiscInertMass = 0.0,     //The mass not accounted for within member components or propellant
				mHeight = 0.0,
				mDiameter = 0.0;

		public:
			IStage() = default;
			virtual ~IStage() = default;

			void update(double t, double dt);

			State& getState() { return mState; }
			const State& getState() const { return mState; }
			FluidTankGroup& getPropellantSupplies() { return mPropellantSupplies; } 
			const FluidTankGroup& getPropellantSupplies() const { return mPropellantSupplies; } 
			ThrustGeneratorGroup& getEngines() { return mEngines; }
			const ThrustGeneratorGroup& getEngines() const { return mEngines; }
			ThrustGeneratorGroup& getThrusters() { return mThrusters; }
			const ThrustGeneratorGroup& getThrusters() const { return mThrusters; }
				
		protected:
			void mergeTotalMass_stage();
			void mergeTotalInertia_stage();

		private:
			virtual void addForces(const State& state, double t);
			virtual void addTorques(const State& state, double t);
			void basicCollision_temp();

			virtual void setStageSpecificParams() = 0;
			virtual void stageSpecificUpdates(double t, double dt) = 0;
			virtual std::vector<Force_world> stageSpecificForces_world() const = 0;
			virtual glm::dvec3 stageSpecificTorques_world() const = 0; //TODO: Should this really be a glm::dvec3?
			virtual Mass stageSpecificMass_stage() const = 0;
			virtual InertiaTensor stageSpecificCoMInertia_stage() const = 0;

		};

	}
}

#endif
