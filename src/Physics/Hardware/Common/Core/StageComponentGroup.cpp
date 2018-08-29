#include "StageComponentGroup.h"

namespace Physics {
	namespace Hardware {

		void StageComponentGroup::addComponent(std::unique_ptr<IStageComponent> component) {
			mComponents.push_back(std::move(component));
			updateAllGroupProperties();
		}

		void StageComponentGroup::updateAllGroupProperties() {
			if (mComponents.empty()) 
				return;
			
			updateTotalMass_stage();
			updateTotalCmInertia_stage();
		}

		void StageComponentGroup::updateTotalMass_stage() {
			//Reset the group centre of mass before recalculation
			mTotalMass_stage.reset();

			for (const auto& c : mComponents)
				mTotalMass_stage += c->getMass(IStageComponent::CoordSpace::stage);
		}

		void StageComponentGroup::updateTotalCmInertia_stage() {
			//Reset the group moment of inertia before recalculation
			mTotalCmInertia_stage.reset();

			for (const auto& c : mComponents)
				mTotalCmInertia_stage += c->getInertia_stage(mTotalMass_stage.getCentre());
		}

	}
}