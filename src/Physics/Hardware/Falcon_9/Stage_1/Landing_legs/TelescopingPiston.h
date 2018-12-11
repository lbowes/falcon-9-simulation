#ifndef PHYSICS_HARDWARE_TELESCOPINGPISTON_H
#define PHYSICS_HARDWARE_TELESCOPINGPISTON_H
#pragma once

#include <PhysicsFramework/Spring.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <algorithm>

namespace Physics {
	class DSS;
}

namespace Physics {
	namespace Hardware {
		
		class TelescopingPiston {
			friend class LandingLeg;
			friend class Physics::DSS;
		private:
			static constexpr double mMaxLength = 10.8;            //m 10.917
			
			static constexpr unsigned char mCylinderCount = 5;
			
			const double mMinLength = 0.0;               //m

			std::vector<double> mSubCylinderLengths;

			Spring mSpring;

			double
				mCurrentLength = 0.0,           //m
				mLastLength = 0.0,              //the length of the piston during the last update step        
				mExtensionRate = 0.0;           //ms^-1

		public:
			TelescopingPiston(double minLength);
			~TelescopingPiston() = default;

			void update(double newLength, double dt);

			double getLength() const { return mCurrentLength; }
			bool isFullyExtended() const { return mCurrentLength >= mMaxLength; }
			double getForceMagnitude() const { return mSpring.getForce(); }
			const std::vector<double>& getSubCylinderLengths() const { return mSubCylinderLengths; }

		private:
			void clampLength();

		};

	}
}

#endif
