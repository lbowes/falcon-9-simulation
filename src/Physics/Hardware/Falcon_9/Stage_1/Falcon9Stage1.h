#ifndef PHYSICS_HARDWARE_FALCON9STAGE1_H
#define PHYSICS_HARDWARE_FALCON9STAGE1_H
#pragma once

#include "Physics/Hardware/Common/Core/IStage.h"
#include "Merlin1D.h"
#include "Physics/Hardware/Common/Propulsion/GasThruster.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Grid_fins/GridFins.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Landing_legs/LandingLegs.h"

#define THRUSTER_SPIN 0
#define LOAD_ENGINE_PROPELLANT 1

namespace Physics {
	namespace Hardware {

		class Falcon9Stage1 : public IStage {
		private:
			LandingLegs mLandingLegs;
			GridFins mGridFins;

		public:
			Falcon9Stage1();
			~Falcon9Stage1() = default;

			void checkInput_temp(double dt);
			
			GridFins& getGridFins() { return mGridFins; }
			const GridFins& getGridFins() const { return mGridFins; }
			LandingLegs& getLandingLegs() { return mLandingLegs; }
			const LandingLegs& getLandingLegs() const { return mLandingLegs; }

		private:
			void preFlight_temp();
			void otherUpdates(double t, double dt);
			std::vector<Force_world> otherForces_world() const;
			glm::dvec3 otherTorques_world() const;
			void addEngines();
			void addThrusters();
			void addTanks();
			void addPropellantLines();
			void setStageSpecificParams();
			Mass otherMass_stage() const;
			InertiaTensor otherCmInertia_stage() const;

		};

	}
}

#endif
