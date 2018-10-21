#ifndef LEGDEPLOYMENTACTUATOR_HPP
#define LEGDEPLOYMENTACTUATOR_HPP
#pragma once

#include "Physics/DynamicSimState.h"

#include <PhysicsFramework/Spring.hpp>

namespace Physics {
	namespace Hardware {

		class LegDeploymentActuator {
		private:
			const double
				mRetractedLength = 0.0,
				mExtensionDistance = 1.0;
			
			Spring mSpring;

		public:
			LegDeploymentActuator(double retractedLength) :
				mRetractedLength(retractedLength),
				mSpring(100000.0, mExtensionDistance, 0.0)
			{ }

			~LegDeploymentActuator() = default;

			void update(double availableExtensionSpace) {
				mSpring.update(std::min(availableExtensionSpace, mExtensionDistance), 0.0);
			}

			void loadDynamicState(const DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& state) {
				mSpring.update(state.springLength, state.springCompressionRate);
			}

			void saveDynamicState(DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& toSaveTo) const {
				toSaveTo.springLength = mSpring.getCurrentLength();
				toSaveTo.springCompressionRate = mSpring.getCompressionRate();
			}

			double getForceMagnitude() {
				return mSpring.getForce();
			}

		};

	}
}

#endif
