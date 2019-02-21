#include "LandingLeg.h"

#include <physics/ChSystemNSC.h>

namespace Physics {
	namespace Hardware {

		const chrono::ChVector2<> LandingLeg::sPos_stage2D =            {1.43521, 0.90617};
		const chrono::ChVector2<> LandingLeg::sPistonStartPos_stage2D = {2.04213, 5.30092};
		const chrono::ChVector2<> LandingLeg::sPusherStartPos_stage2D = {1.80105, 5.20809};
		const chrono::ChVector2<> LandingLeg::sPistonEndPos_AFrame2D =  {0.57500, 8.70000};
		const chrono::ChVector2<> LandingLeg::sCoMPos_AFrame2D =        {0.44760, 4.52721};
		const chrono::ChVector2<> LandingLeg::sPusherEndPos_AFrame2D =  {0.85000, 3.90000};

		unsigned char LandingLeg::sInstanceCount = 0;

		LandingLeg::LandingLeg(chrono::ChBodyAuxRef& stage1Body, double clockingDegree) :
			mClockingDegree(clockingDegree),
			mStage1Body(stage1Body),
			mSystemHandle(stage1Body.GetSystem())
		{
			//mAFrameBody = std::make_shared<chrono::ChBodyAuxRef>();
			//mSystemHandle->AddBody(mAFrameBody);

			//mAFrameBody->SetNameString("Landing leg " + std::to_string(sInstanceCount) + " A-frame");
			//mAFrameBody->SetFrame_COG_to_REF({0.0, sCoMPos_AFrame2D.y, -sCoMPos_AFrame2D.x});
			//mAFrameBody->SetMass(sAFrameMass);
			//mAFrameBody->SetCollide(true);
			//mAFrameBody->SetInertia(calcInertia_AFrame());

			attachToStage();

			sInstanceCount++;
		}

		void LandingLeg::attachToStage()
			// TODO:
			// Link the landing leg system to the body of the stage
		{
			//mAFrameJoint = std::make_shared<chrono::ChLinkLockRevolute>();
			//mSystemHandle->AddLink(mAFrameJoint);
			
			// TODO: Create the joint coordinate frame in stage space.
			//chrono::ChFrame<> jointFrame_stage;
			//jointFrame_stage.SetPos({});

			// TODO: create the joint coordinate frame in A-frame space.
			//chrono::ChFrame<> jointFrame_AFrame;
			//jointFrame_AFrame.SetPos({});

			//mAFrameJoint->Initialize(mStage1Body, mAFrameBody, true, jointFrame_stage, jointFrame_AFrame);
		}

		chrono::ChMatrix33<> LandingLeg::calcInertia_AFrame() const {
			chrono::ChMatrix33<> localInertia;
			
			// TODO: Calculate/estimate the inertia tensor of the A-frame w.r.t its origin.
			
			return localInertia;
		}

	}
}