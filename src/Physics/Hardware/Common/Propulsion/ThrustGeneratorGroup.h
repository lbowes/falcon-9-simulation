#ifndef PHYSICS_HARDWARE_THRUSTGENERATORGROUP_H
#define PHYSICS_HARDWARE_THRUSTGENERATORGROUP_H
#pragma once

#include "IThrustGenerator.h"
#include "Physics/Hardware/Common/Core/StageComponentGroup.h"

namespace Physics {
	namespace Hardware {
		
		class ThrustGeneratorGroup : public StageComponentGroup {
		private:
			glm::dvec3 mTotalForce_stage;

		public:
			ThrustGeneratorGroup() = default;
			virtual ~ThrustGeneratorGroup() = default;
			
			void update(double dt);
			
			glm::dvec3 getTotalForce_stage() const { return mTotalForce_stage; }

		private:
			void updateAllDevices(double dt) const;
			void updateTotalForce_stage();

		};

	}
}

#endif	
