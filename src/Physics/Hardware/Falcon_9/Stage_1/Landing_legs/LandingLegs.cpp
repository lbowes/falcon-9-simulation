#include "LandingLegs.h"

namespace Physics {
	namespace Hardware {

		LandingLegs::LandingLegs() {
			addLegs();
		}

		void LandingLegs::update(const RigidBody& stage, double dt) {
			updateAllLegs(stage, dt);
			updateAllGroupProperties();
		}

		void LandingLegs::addLegs() {
			for (unsigned char i = 0; i < 4; i++)
				addComponent(std::make_unique<LandingLeg>(45.0 + i * 90.0));
		}

		void LandingLegs::updateAllLegs(const RigidBody& stage, double dt) const {
			for (const auto& c : mComponents) {
				glm::dvec3 legOriginAccel_world = stage.getAccelAtLocalPoint_world(c->getCompToStageTransform().toParentSpace_translation());
				static_cast<LandingLeg*>(c.get())->update(stage.getState().getObjectSpace(), legOriginAccel_world, dt);
			}
		}

	}
}