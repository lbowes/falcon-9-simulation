#include "Merlin1D.h"

//temp
#include <iostream>
//

namespace Physics {
	namespace Hardware {
		
		Merlin1D::Merlin1D(CoordTransform3D merlinToStage) :
			Engine(merlinToStage)
		{
			setTypeSpecificParams();
		}

		void Merlin1D::setTypeSpecificParams() {
			//TODO - Check that all engine-specific properties for the Merlin 1D are accurate 

			//mVe_SL = mDirection_W * 2765.4753;
			//mVe_Vac = mDirection_W * 3049.86815;
			mMaxGimbalAngle = 6.0;           //6.0
			mPeakThrustMagnitude = 845'000.0; //845000.0 -> block 4, 845162.11 -> block 5
			mMass_comp = { 470.0, { 0.0, -1.1, 0.0 } };              //470.0
			mChamberPressure = 9'700'000.0;    //9700000.0
			mNozzleExitDiameter = 0.97;      //0.97
			mNozzleLength = 1.64667; //TODO - THIS IS NOT CORRECT. IT IS A PLACEHOLDER.
			mTotalLength = 1.74997;
			mNozzleThroatDiameter = 0.12125; //0.12125
			mPeakMassFlowRate = 302.6168812; //302.6168812
			mOFMixtureRatio = 287430.0 / 123570.0/*2.32605001214*/;
			mThrottleMin = 0.55;             //0.55
			mThrottleMax = 1.0;              //1.0
			mExpansionRatio = 16.0;          //16.0

			updateProperties();

			//addTVCActuator(glm::dvec2(0.4, 0.0), glm::dvec2(0.16, -0.4), 180.0);
			//addTVCActuator(glm::dvec2(0.4, 0.0), glm::dvec2(0.16, -0.4), 90.0);
			mTVCActuators.push_back({180.0});
			mTVCActuators.push_back({90.0});
		}

		void Merlin1D::gimbalTo(double clockDirAngle_degs, double angle_degs) 
			//Responsible for taking in gimbal parameters and converting these into correct TVC actuator commands 
		{
			const double 
				angle_rads = std::clamp<double>(glm::radians(angle_degs), 0.0, glm::radians(mMaxGimbalAngle)),
				clockDirAngle_rads = glm::radians(clockDirAngle_degs);

			double 
				x_rads = asin(sin(angle_rads) * sin(glm::half_pi<double>() - clockDirAngle_rads)),
				z_rads = asin(sin(angle_rads) * sin(clockDirAngle_rads));       

			mTVCActuators[0].mGimbalAngle = glm::degrees(x_rads);
			mTVCActuators[1].mGimbalAngle = glm::degrees(z_rads);
		}

	}
}