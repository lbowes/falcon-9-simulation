#include "StageComponentGroup.h"

namespace Physics {
	namespace Hardware {

		void StageComponentGroup::addComponent(std::unique_ptr<StageComponent> component) {
			mComponents.push_back(std::move(component));
			updateAllGroupProperties();
		}

		void StageComponentGroup::updateAllGroupProperties() {
			if (mComponents.empty()) 
				return;
			
			updateTotalMass_stage();
			updateTotalCoMInertia_stage();
		}

		void StageComponentGroup::updateTotalMass_stage() {
			//Reset the group centre of mass before recalculation
			mTotalMass_stage.reset();

			for (const auto& c : mComponents)
				mTotalMass_stage += c->getMass(StageComponent::CoordSpace::stage);
		}

		void StageComponentGroup::updateTotalCoMInertia_stage() {
			//Reset the group moment of inertia before recalculation
			mTotalCoMInertia_stage.reset();

			for (const auto& c : mComponents)
				mTotalCoMInertia_stage += c->getInertia_stage(mTotalMass_stage.getCentre());
		}

	}
}