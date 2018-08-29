#include "ThrustGeneratorGroup.h"

namespace Physics {
	namespace Hardware {

		void ThrustGeneratorGroup::update(double dt) {
			/* This check is crucial. If no devices have been added (which could reasonably occur) then
			getMass() (called when centre of mass and inertia tensor are being recalculated) returns
			0, as there is literally no mass. Components are then divided by zero and they become
			-nan(ind). Stuff breaks. */
			if (!mComponents.empty()) {
				updateAllDevices(dt);
				updateTotalForce_stage();
				updateTotalMass_stage();
				updateTotalCmInertia_stage();
			}
		}

		void ThrustGeneratorGroup::updateAllDevices(double dt) const {
			for (const auto& c : mComponents)
				static_cast<IThrustGenerator*>(c.get())->update(dt);
		}

		void ThrustGeneratorGroup::updateTotalForce_stage() {
			glm::dvec3 output;

			IThrustGenerator* temp = nullptr;
			for (const auto& c : mComponents) {
				temp = static_cast<IThrustGenerator*>(c.get());

				if (temp->isActive())
					output += temp->getThrust_stage();
			}

			mTotalForce_stage = output;
		}

	}
}