#include "IStage.h"

#include <physics/ChSystemNSC.h>
#include <physics/ChBodyEasy.h>

namespace Physics {
	namespace Hardware {
		
		IStage::IStage(chrono::ChSystemNSC& sys, chrono::ChFrame<> stage_to_LV) :
			mSystemHandle(sys),
			mBody(std::make_shared<chrono::ChBodyAuxRef>()),
			mStage_to_LV(stage_to_LV)
		{
			mSystemHandle.AddBody(mBody);

			mBody->GetCollisionModel()->SetFamily(2);
			mBody->GetCollisionModel()->SetFamilyMaskNoCollisionWithFamily(3);
		}

		void IStage::update(double dt) {
			// 1. Update all vehicle subsystems
			// mEngines.update(dt)
			// mRCSThrusters.update(dt)
			// mPropSupplies.update(dt);
			stageSpecificUpdates(dt);
			
			// 2. Calculate and set rigid body properties
			// 
		}

		void IStage::assemble() {
			addMiscMass();
			addPropellantSupplies();
			addEngines();
			// TODO:
			// addEngines()
			// addSomethingElse()
		}

	}
}