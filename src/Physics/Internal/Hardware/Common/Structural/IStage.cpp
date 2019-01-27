#include "IStage.h"

#include <physics/ChSystemNSC.h>

//temp
#include <chrono/physics/ChBodyEasy.h>
//

namespace Physics {
	namespace Hardware {
		
		IStage::IStage(chrono::ChSystemNSC& sys, const chrono::ChFrame<> stage_to_LV) :
			mSystemHandle(sys),
			mStage_to_LV(stage_to_LV)
		{
			mBody = std::make_shared<chrono::ChBodyAuxRef>();

			//temp
			auto rigidFloor = std::make_shared<chrono::ChBodyEasyBox>(250, 4, 250,  // x,y,z size
                                                       1000,         // density
                                                       true,         // collide enable?
                                                       false);
			
			chrono::collision::ChCollisionModel::SetDefaultSuggestedEnvelope(1.0);
			chrono::collision::ChCollisionModel::SetDefaultSuggestedMargin(0.5);

			rigidFloor->SetBodyFixed(true);

			mSystemHandle.AddBody(rigidFloor);
			//
		}

		void IStage::update(double dt) {
			// 1. Update all vehicle subsystems
			// mEngines.update(dt)
			// mRCSThrusters.update(dt)
			// mPropSupplies.update(dt);
			// stageSpecificUpdates(dt);
			
			// 2. Calculate and set rigid body properties
			// 
		}

	}
}