#ifndef PHYSICS_HARDWARE_ISTAGE_H
#define PHYSICS_HARDWARE_ISTAGE_H
#pragma once

#include "../Propellant/CylinderFluidTankGroup.h"

#include <physics/ChBodyAuxRef.h>

namespace chrono {
	class ChSystemNSC;
}

namespace Physics {
	namespace Hardware {

		// Provides an interface used to aid in the creation of launch vehicle stages.
		class IStage {
		protected:
			chrono::ChSystemNSC& mSystemHandle;
			std::shared_ptr<chrono::ChBodyAuxRef> mBody;

			// TODO: could the stage be built up of multiple subsystems?
			// Subsystems could act independently of one another and the stage object (base or subclass) could be
			// responsible for messaging between subsystems and combining their state (calculating combined mass,
			// inertia etc). Examples could include EngineSystem, PropellantSystem, ReactionControlSystem.
			// Each subsystem would be responsible for defining its own properties and owning its own rigid bodies or
			// fixed components and would need access to the main stage mBody (above).
			CylinderFluidTankGroup mPropSupplies;

			chrono::ChFrame<> mStage_to_LV;

		public:
			IStage(chrono::ChSystemNSC& sys, chrono::ChFrame<> stage_to_LV = chrono::ChFrame<>());
			virtual ~IStage() = default;

			void update(double dt);

		protected:
			void assemble();
			virtual void stageSpecificUpdates(double dt) = 0;

			virtual void addPropellantSupplies() = 0;
			//virtual void addEngines() = 0;

		};

	}
}

#endif
