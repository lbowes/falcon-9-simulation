#ifndef PHYSICS_HARDWARE_ENGINE_H
#define PHYSICS_HARDWARE_ENGINE_H
#pragma once

#include "IThrustGenerator.h"
#include "Physics/Hardware/Common/Propellant/SupplyLine.h"

#include <vector>

namespace Graphics {
	class ExhaustJet;
}

namespace Physics{
	class DSS;
}

namespace Physics {
	namespace Hardware {
		
		struct TVCActuator {
			const double mClockingDegree = 0.0;
			double mGimbalAngle = 0.0;
		};

		class Engine : public IThrustGenerator {
			friend class Graphics::ExhaustJet;
			friend class Physics::DSS;
		protected:
			const CoordTransform3D mEngineToStageNoGimbal;

			PropSupplyLine* mSupplyLine = nullptr;

			std::vector<TVCActuator> mTVCActuators;
			
			//TODO: Very few members below are currently used. Remove unnecessary members here and complete
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

		public:
			Engine(CoordTransform3D engineToStage);
			virtual ~Engine() = default;

			void setThrottle(double newThrottle);
			double getThrottle() const { return mThrottle; }
			double getThrottleMin() const { return mThrottleMin; }
			double getThrottleMax() const { return mThrottleMax; }
			double getNozzleExitDiameter() const { return mNozzleExitDiameter; }
			void attachPropSupplyLine(PropSupplyLine* supplyLine) { mSupplyLine = supplyLine; }

		protected:
			void updateProperties();

		private:
			void updateDeviceSpecific(double dt);
			void updateGimbalRotation();
			virtual void setTypeSpecificParams() = 0;

		};

	}
}

#endif
