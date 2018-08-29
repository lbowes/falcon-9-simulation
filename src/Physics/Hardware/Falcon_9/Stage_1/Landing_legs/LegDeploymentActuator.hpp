#ifndef LEGDEPLOYMENTACTUATOR_H
#define LEGDEPLOYMENTACTUATOR_H
#pragma once

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

			double getForceMagnitude() {
				return mSpring.getForce();
			}

		};

	}
}

#endif
