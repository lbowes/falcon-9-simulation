#ifndef PHYSICS_HARDWARE_STAGECOMPONENTGROUP_H
#define PHYSICS_HARDWARE_STAGECOMPONENTGROUP_H
#pragma once

#include "Physics/Hardware/Common/Core/IStageComponent.hpp"

#include <vector>
#include <memory>

namespace Physics {
	namespace Hardware {
		
		class StageComponentGroup {
		protected:
			std::vector<std::unique_ptr<IStageComponent>> mComponents;
		
		private:
			glm::dvec3 mAvgPosition_stage;
			Mass mTotalMass_stage;
			InertiaTensor mTotalCmInertia_stage;

		public:
			StageComponentGroup() = default;
			virtual ~StageComponentGroup() = default;

			void addComponent(std::unique_ptr<IStageComponent> component);

			IStageComponent* operator[](unsigned index) const { return getComponent(index); }
			IStageComponent* getComponent(unsigned index) const { return index < mComponents.size() ? mComponents[index].get() : (IStageComponent*)nullptr; }
			
			template <class T>
			T* getComponent(const unsigned index) const { return index < mComponents.size() ? static_cast<T*>(mComponents[index].get()) : (T*)nullptr; }

			std::vector<std::unique_ptr<IStageComponent>>& getAllComponents() { return mComponents; }
			const std::vector<std::unique_ptr<IStageComponent>>& getAllComponents() const { return mComponents; }
			unsigned getCount() const { return static_cast<unsigned>(mComponents.size()); }
			const Mass& getTotalMass_stage() const { return mTotalMass_stage; }
			const InertiaTensor& getTotalCmInertia_stage() const { return mTotalCmInertia_stage; }

		protected: 
			void updateAllGroupProperties();
			void updateTotalMass_stage();
			void updateTotalCmInertia_stage();

		};

	}
}

#endif
