#ifndef PHYSICS_HARDWARE_LEGDEPLOYMENTACTUATOR_HPP
#define PHYSICS_HARDWARE_LEGDEPLOYMENTACTUATOR_HPP
#pragma once

#include <PhysicsFramework/Spring.hpp>

namespace Physics {
	class DSS;
}

namespace Physics {
	namespace Hardware {

		class LegDeploymentActuator {
			friend class Physics::DSS;
		private:
			const double
				mRetractedLength = 0.0,
				mExtensionDistance = 1.47; //1.47
			
			Spring mSpring;

		public:
			LegDeploymentActuator(double retractedLength) :
				mRetractedLength(retractedLength),
				mSpring(100000.0, mExtensionDistance, 0.0)
			{ }

			~LegDeploymentActuator() = default;

			double getLength() const { return mSpring.getCurrentLength(); }

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
