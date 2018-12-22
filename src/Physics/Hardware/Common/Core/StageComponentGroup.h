#ifndef PHYSICS_HARDWARE_STAGECOMPONENTGROUP_H
#define PHYSICS_HARDWARE_STAGECOMPONENTGROUP_H
#pragma once

#include "Physics/Hardware/Common/Core/StageComponent.hpp"

#include <vector>
#include <memory>

namespace Physics {
	namespace Hardware {
		
		class StageComponentGroup {
		protected:
			std::vector<std::unique_ptr<StageComponent>> mComponents;
		
		private:
			glm::dvec3 mAvgPosition_stage;
			Mass mTotalMass_stage;
			InertiaTensor mTotalCoMInertia_stage;

		public:
			StageComponentGroup() = default;
			virtual ~StageComponentGroup() = default;

			void addComponent(std::unique_ptr<StageComponent> component);

			StageComponent* operator[](unsigned index) const { return get(index); }
			StageComponent* get(unsigned index) const { return index < mComponents.size() ? mComponents[index].get() : (StageComponent*)nullptr; }
			
			template <class T>
			T* get(const unsigned index) const { return index < mComponents.size() ? static_cast<T*>(mComponents[index].get()) : (T*)nullptr; }

			std::vector<std::unique_ptr<StageComponent>>& getAllComponents() { return mComponents; }
			const std::vector<std::unique_ptr<StageComponent>>& getAllComponents() const { return mComponents; }
			unsigned getCount() const { return static_cast<unsigned>(mComponents.size()); }
			const Mass& getTotalMass_stage() const { return mTotalMass_stage; }
			const InertiaTensor& getTotalCoMInertia_stage() const { return mTotalCoMInertia_stage; }

		protected: 
			void updateAllGroupProperties();
			void updateTotalMass_stage();
			void updateTotalCoMInertia_stage();

		};

	}
}

#endif
