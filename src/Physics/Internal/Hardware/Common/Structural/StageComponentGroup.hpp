#ifndef PHYSICS_HARDWARE_STAGECOMPONENTGROUP_H
#define PHYSICS_HARDWARE_STAGECOMPONENTGROUP_H
#pragma once

#include "IStageComponent.hpp"

#include <vector>
#include <memory>

namespace Physics {
	namespace Hardware {

		class StageComponentGroup {
		protected:
			std::vector<std::unique_ptr<IStageComponent>> mComponents;

		public:
			StageComponentGroup() = default;
			virtual ~StageComponentGroup() = default;

			void addComponent(std::unique_ptr<IStageComponent> component) {
				mComponents.push_back(std::move(component));
			}

			//TODO: Decide what the basic function of StageComponentGroup should be.
			//Won't the functions below result in lots of recomputation? Should subclasses define these implementations in order to
			//store data and only change it when necessary (similar to IStageComponent at the moment)?

			chrono::ChMatrix33<> getCombinedInertia_stage() const {
				chrono::ChMatrix33<> result;

				for(const auto& c : mComponents)
					result += c->getInertia_stage();

				return result;
			}

			double getCombinedMass() const {
				double combinedMass = 0.0;

				for(const auto& c : mComponents)
					combinedMass += c->getMass();

				return combinedMass;
			}

			chrono::ChVector<> getCombinedCoM_stage() const {
				chrono::ChVector<> combinedCoM_stage;
				double combinedMass = 0.0;

				for(const auto& c : mComponents) {
					combinedCoM_stage += c->getCoM_stage() * c->getMass();			
					combinedMass += c->getMass();
				}

				combinedCoM_stage /= combinedMass;
			}

		};

	}
}

#endif