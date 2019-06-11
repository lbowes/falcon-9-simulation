#ifndef PHYSICS_HARDWARE_ISTAGE_H
#define PHYSICS_HARDWARE_ISTAGE_H
#pragma once

#include "../Propellant/PropellantSubsystem.h"

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
			chrono::ChFrame<> mStage_to_LV;

			PropellantSubsystem mPropSupplies;
			// EngineSubsystem mEngines;
			// ReactionControlSystem mRCS;
			// AvionicsSubsystem mAvionics;

		public:
			IStage(chrono::ChSystemNSC& sys, chrono::ChFrame<> stage_to_LV = chrono::ChFrame<>());
			virtual ~IStage() = default;

			void update(double dt);

		protected:
			void assemble();
			virtual void stageSpecificUpdates(double dt) = 0;
			virtual void addMiscMass() = 0;
			virtual void addPropellantSupplies() = 0;
			virtual void addEngines() = 0;

		};

	}
}

#endif // PHYSICS_HARDWARE_ISTAGE_H
