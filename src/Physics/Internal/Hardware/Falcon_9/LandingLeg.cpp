#include "LandingLeg.h"

#include <physics/ChSystemNSC.h>
#include <physics/ChInertiaUtils.h>
#include <chrono/utils/ChUtilsGeometry.h>

namespace Physics {
	namespace Hardware {

		const chrono::ChVector2<> LandingLeg::sPos_stage2D =            { 1.43521, 0.90617 };
		const chrono::ChVector2<> LandingLeg::sPistonStartPos_stage2D = { 2.04213, 5.30092 };
		const chrono::ChVector2<> LandingLeg::sPusherStartPos_stage2D = { 1.80105, 5.20809 };
		const chrono::ChVector2<> LandingLeg::sPistonEndPos_AFrame2D =  { 0.57500, 8.70000 };
		const chrono::ChVector2<> LandingLeg::sCoMPos_AFrame2D =        { 0.44760, 4.52721 };
		const chrono::ChVector2<> LandingLeg::sPusherEndPos_AFrame2D =  { 0.85000, 3.90000 };

		unsigned char LandingLeg::sInstanceCount = 0;

		LandingLeg::LandingLeg(chrono::ChSystemNSC* systemHandle) :
            mSystemHandle(systemHandle)
        {
			mAFrameBody = std::make_shared<chrono::ChBodyAuxRef>();
			systemHandle->AddBody(mAFrameBody);

			mAFrameBody->SetNameString("Landing leg " + std::to_string(sInstanceCount) + " A-frame");
			mAFrameBody->SetFrame_COG_to_REF({0.0, sCoMPos_AFrame2D.y(), -sCoMPos_AFrame2D.x()});
			mAFrameBody->SetMass(sAFrameMass);
			mAFrameBody->SetInertia(calcInertia_AFrame());
			mAFrameBody->SetCollide(true);

            // TODO: Collision shape

			sInstanceCount++;
		}

		void LandingLeg::attachToStage(std::shared_ptr<chrono::ChBody> mount, double clockingDegree_degs) {
			mAFrameJoint = std::make_shared<chrono::ChLinkLockRevolute>();
			mSystemHandle->AddLink(mAFrameJoint);
			
			// The joint coordinate frame in stage space.
			chrono::ChFrame<> jointFrame_stage;
            chrono::Vector pos_stage3D = { 0.0, sPos_stage2D.y(), -sPos_stage2D.x() };
            const chrono::ChQuaternion<> clockingRotation = chrono::Q_from_AngY(clockingDegree_degs * chrono::CH_C_DEG_TO_RAD);
            pos_stage3D = clockingRotation.Rotate(pos_stage3D);
            jointFrame_stage.SetPos(pos_stage3D);
            jointFrame_stage.SetRot(clockingRotation);

			// Joint coordinate frame in A-frame space.
			chrono::ChCoordsys<> jointFrame_AFrame;

            // Join the A frame to the stage
			mAFrameJoint->Initialize(mount, mAFrameBody, true, jointFrame_stage.GetCoord(), jointFrame_AFrame);
		}

		chrono::ChMatrix33<> LandingLeg::calcInertia_AFrame() const {
			using namespace chrono;
            
            // Calculate inertia of landing leg about its centre of mass
            ChMatrix33<> 
                inertia_CoM = utils::CalcCylinderGyration(0.4032, 3.4715),
                inertia_origin;
                
            // Calculate the 3D displacement vector of the centre of mass from the leg's origin
            const ChVector<> CoMPos_AFrame3D = { 0.0, sCoMPos_AFrame2D.y(), -sCoMPos_AFrame2D.x() };

            // Use the parallel axis theorem to calculate the inertia of the leg about its origin
            ChInertiaUtils::TranslateInertia(inertia_CoM, -CoMPos_AFrame3D, sAFrameMass, inertia_origin);

			return inertia_origin;
		}

	}
}