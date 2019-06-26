#include "GroundPlane.h"

#include <chrono/physics/ChSystemNSC.h>
#include <physics/ChBodyEasy.h>

namespace Physics {
    namespace External {

		const chrono::Vector GroundPlane::mPos_world = chrono::ChVector<>();

		const float GroundPlane::mThickness = 3;

        GroundPlane::GroundPlane(chrono::ChSystemNSC& systemHandle) :
            mSystemHandle(systemHandle)
        {
		    // Initialise body and add it to system
		    mBody = std::make_shared<chrono::ChBody>();
            systemHandle.AddBody(mBody);

            // Collision
            mBody->GetCollisionModel()->SetEnvelope(2.0);
		    mBody->GetCollisionModel()->SetSafeMargin(2.0);
		    mBody->GetCollisionModel()->ClearModel();
		    mBody->GetCollisionModel()->AddBox(25, mThickness, 25);
		    mBody->GetCollisionModel()->BuildModel();
		    mBody->SetBodyFixed(true);
			mBody->SetCollide(true);

            // Material properties
    		mBody->GetMaterialSurfaceNSC()->SetFriction(1);
            mBody->GetMaterialSurfaceNSC()->SetRestitution(0);
            mBody->GetMaterialSurfaceNSC()->SetFriction(1);
    		mBody->GetMaterialSurfaceNSC()->SetRollingFriction(1);
    		mBody->GetMaterialSurfaceNSC()->SetSpinningFriction(1);

    		// Positioning in world
    		mBody->SetPos(mPos_world - chrono::Vector(0, mThickness, 0));
        }

		const chrono::Vector& GroundPlane::getPos_world() { return mPos_world; }

    }
}