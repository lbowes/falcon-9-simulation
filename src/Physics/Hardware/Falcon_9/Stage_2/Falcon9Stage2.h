#ifndef PHYSICS_HARDWARE_FALCON9STAGE2_H
#define PHYSICS_HARDWARE_FALCON9STAGE2_H
#pragma once

#include "Physics/Hardware/Common/Core/IStage.h"
#include "Merlin1DVac.h"
#include "Physics/Hardware/Common/Propulsion/GasThruster.h"

namespace Physics {
	namespace Hardware {
		
		class Falcon9Stage2 : public IStage {
		public:
			Falcon9Stage2();
			~Falcon9Stage2() = default;

			void preFlight_temp();

		private:
			void otherUpdates(double t, double dt);
			std::vector<Force_world> otherForces_world() const;
			glm::dvec3 otherTorques_world() const;
			void addEngine();
			void addThrusters();
			void addTanks();
			void setStageSpecificParams();
			Mass otherMass_stage() const;
			InertiaTensor otherCmInertia_stage() const;

		};

	}
}

#endif
