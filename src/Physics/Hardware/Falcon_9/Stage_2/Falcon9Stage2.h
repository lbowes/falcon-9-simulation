#ifndef PHYSICS_HARDWARE_FALCON9STAGE2_H
#define PHYSICS_HARDWARE_FALCON9STAGE2_H
#pragma once

#include "Physics/Hardware/Common/Core/IStage.h"
#include "Merlin1DVac.h"
#include "Physics/Hardware/Common/Propulsion/GasThruster.h"
#include "FairingHalf.h"

namespace Physics {
	namespace Hardware {
		
		class Falcon9Stage2 : public IStage {
		private:
			std::pair<FairingHalf, FairingHalf> mFairingHalves = { FairingHalf(180.0), FairingHalf(0.0) };

		public:
			Falcon9Stage2();
			~Falcon9Stage2() = default;

			const std::pair<FairingHalf, FairingHalf>& getFairings() const { return mFairingHalves; }

		private:
			void preFlight_temp();
			void stageSpecificUpdates(double t, double dt);
			std::vector<Force_world> stageSpecificForces_world() const;
			glm::dvec3 stageSpecificTorques_world() const;
			void addEngine();
			void addThrusters();
			void addTanks();
			void setStageSpecificParams();
			Mass stageSpecificMass_stage() const;
			InertiaTensor stageSpecificCoMInertia_stage() const;

		};

	}
}

#endif
