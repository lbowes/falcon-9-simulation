#ifndef PHYSICS_HARDWARE_ENGINE_H
#define PHYSICS_HARDWARE_ENGINE_H
#pragma once

#include "IThrustGenerator.h"
#include "Physics/Hardware/Common/Propellant/SupplyLine.h"

namespace Graphics {
	class ExhaustJet;
}

namespace Physics {
	namespace Hardware {
		
		class Engine : public IThrustGenerator {
			friend class Graphics::ExhaustJet;
		protected:
			PropSupplyLine* mSupplyLine = nullptr;
			
			//TODO: Very few members below are currently used. Remove unnecessary members here and complete

			glm::dvec3
				mVe_SL,		                 //EFFECTIVE exhaust velocity at sea level
				mVe_Vac;		             //EFFECTIVE exhaust velocity in vacuum

			double
				mMaxGimbalAngle = 0.0,       //degrees
				mChamberPressure = 0.0,      //Pa
				mExpansionRatio = 0.0,	     //m
				mNozzleExitDiameter = 0.0,	 //m
				mNozzleThroatDiameter = 0.0, //m
				mNozzleExitArea = 0.0,	     //ms^2
				mNozzleThroatArea = 0.0,	 //ms^2
				mNozzleLength = 0.0,         //m
				mTotalLength = 0.0,          //m
				mThrottle = 1.0,		     //Percentage throttle
				mPeakThrustMagnitude = 0.0,  //N
				mPeakMassFlowRate = 0.0,     //(mean between sea level and vacuum)
				mCurrentMassFlowRate = 0.0,  //kg/s
				mOFMixtureRatio = 0.0,       //oxygen : fuel
				mThrottleMin = 0.0,	         //Minimum percentage throttle
				mThrottleMax = 0.0;	         //Maximum percentage throttle
				//mVe_SL = 282.0,            //s
				//mVe_Vac = 311.0,           //s

		public:
			Engine(GF::CoordTransform3D engineToStage);
			virtual ~Engine() = default;

			double getThrottle() const { return mThrottle; }
			void setThrottle(double newThrottle);
			void attachPropSupplyLine(PropSupplyLine* supplyLine) { mSupplyLine = supplyLine; }

		protected:
			void updateProperties();

		private:
			void updateDeviceSpecific(double dt);
			void updateGimbalAngle();
			virtual void setTypeSpecificParams() = 0;

		};

	}
}

#endif
