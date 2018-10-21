#include "GridFins.h"

namespace Physics {
	namespace Hardware {

		GridFins::GridFins() { 
			addFins();
		}

		void GridFins::addFins() {
			addComponent(std::make_unique<GridFin>(45.0));
			addComponent(std::make_unique<GridFin>(135.0));
			addComponent(std::make_unique<GridFin>(225.0));
			addComponent(std::make_unique<GridFin>(315.0));
		}

		void GridFins::update(double dt/* , double fluidDensity, glm::dvec3 flowVelocity_stage */) {
			if (!mComponents.empty()) {
				//All aerodynamic models of all grid fins (all their reference areas etc) are updated based on flow velocity
				updateAllFins(dt/* , fluidDensity, flowVelocity_stage */);

				//updateArea();
				//updateCentrePressure_stage();

				updateAllGroupProperties();
			}
		}

		void GridFins::loadDynamicState(const DSS::Falcon9::Stage1& stage1State) {
			for(unsigned char i = 0; i < 4; i++)
				static_cast<GridFin*>(mComponents[i].get())->loadDynamicState(stage1State.gridFins[i]);
		}

		void GridFins::saveDynamicState(DSS::Falcon9::Stage1& toSaveTo) const {
			for(unsigned char i = 0; i < 4; i++)
				static_cast<GridFin*>(mComponents[i].get())->saveDynamicState(toSaveTo.gridFins[i]);
		}

		void GridFins::updateAllFins(double dt/* , double fluidDensity, glm::dvec3 flowVelocity_stage */) const {
			for (const auto& c : mComponents)
				static_cast<GridFin*>(c.get())->update(dt/* , fluidDensity, flowVelocity_stage */);
		}

		void GridFins::updateTotalForce_world() {
			mTotalForce_world = glm::dvec3(0.0);
			
			//TODO: Add the aerodynamic force generated by the grid fins
		}

		void GridFins::updateTotalTorque_stage() {
			mTotalTorque_stage = glm::dvec3(0.0);

			//TODO: Add the aerodynamic torque generated by the grid fins
		}

	}
}