#ifndef PHYSICS_HARDWARE_FALCON9STAGE1_H
#define PHYSICS_HARDWARE_FALCON9STAGE1_H
#pragma once

#include "Physics/Hardware/Common/Core/IStage.h"
#include "Merlin1D.h"
#include "Physics/Hardware/Common/Propulsion/GasThruster.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Grid_fins/GridFins.h"
#include "Physics/Hardware/Falcon_9/Stage_1/Landing_legs/LandingLegs.h"
#include "definitions.h"

namespace Physics {
	namespace Hardware {

		class Falcon9Stage1 : public IStage {
		private:
			LandingLegs mLandingLegs;
			GridFins mGridFins;

		public:
			Falcon9Stage1();
			~Falcon9Stage1() = default;

			GridFins& getGridFins() { return mGridFins; }
			const GridFins& getGridFins() const { return mGridFins; }
			LandingLegs& getLandingLegs() { return mLandingLegs; }
			const LandingLegs& getLandingLegs() const { return mLandingLegs; }

		private:
			void preFlight_temp();
			void stageSpecificUpdates(double t, double dt);
			std::vector<Force_world> stageSpecificForces_world() const;
			glm::dvec3 stageSpecificTorques_world() const;
			void addEngines();
			void addThrusters();
			void addTanks();
			void addFluidLines();
			void setStageSpecificParams();
			Mass stageSpecificMass_stage() const;
			InertiaTensor stageSpecificCoMInertia_stage() const;

		};

	}
}

#endif
