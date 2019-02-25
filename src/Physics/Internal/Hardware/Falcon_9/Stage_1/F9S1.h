#ifndef PHYSICS_HARDWARE_FALCON9STAGE1_H
#define PHYSICS_HARDWARE_FALCON9STAGE1_H
#pragma once

#include "../../Common/Structural/IStage.h"

namespace chrono {
	class ChSystemNSC;
}

namespace Physics {
	class F9S1_DSS;
	
	namespace Hardware {

		class F9S1 : public IStage {
			friend class Physics::F9S1_DSS;
		private:

		public:
			F9S1(chrono::ChSystemNSC& sys);
			~F9S1() = default;

			void stageSpecificUpdates(double dt) override;

		private:
			void addPropellantSupplies() override;

		};

	}

	class F9S1_DSS {
	private:
		//TODO: Add all dynamic state owned by F9S1
		chrono::ChFrame<> 
			mS1ToWorldTransform,
			mTankToWorldTransform;

	public:
		F9S1_DSS() = default;
		F9S1_DSS(const Hardware::F9S1& f9s1);
		void operator=(const F9S1_DSS& other);
		~F9S1_DSS() = default;

		static F9S1_DSS lerp(const F9S1_DSS& a, const F9S1_DSS& b, float x);

		const chrono::ChFrame<>& getS1ToWorldTransform() const { return mS1ToWorldTransform; }
		const chrono::ChFrame<>& getTankToWorldTransform() const { return mTankToWorldTransform; }

	};	

}

#endif
