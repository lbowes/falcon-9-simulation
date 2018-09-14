#ifndef PHYSICS_HARDWARE_TELESCOPINGPISTON_H
#define PHYSICS_HARDWARE_TELESCOPINGPISTON_H
#pragma once

#include "PistonCylinder.h"

#include <PhysicsFramework/Spring.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <algorithm>

namespace Physics {
	namespace Hardware {
		
		class TelescopingPiston {
			friend class LandingLeg;
		private:
			static constexpr double
				mMaxLength = 10.917, //m
				mMaxWidth = 1.15,    //m
				mMinWidth = 0.7;     //m
			
			static constexpr unsigned char mCylinderCount = 5;

			const glm::dvec3 mMountPoint_stage;
			
			const double
				mClockingDegree_degs = 0.0,     //degs
				mMinLength = 0.0;               //m

			std::vector<PistonCylinder> mCylinders;

			Spring mSpring;

			double
				mCurrentLength = 0.0,           //m
				mAngleFromVertical_stage = 0.0, //degs
				mLastLength = 0.0,              //the length of the piston during the last update step        
				mExtensionRate = 0.0;           //ms^-1

		public:
			TelescopingPiston(glm::dvec3 mountPoint_stage, double clockingDegree_degs, double minLength);
			~TelescopingPiston() = default;

			PistonCylinder* getCylinder(unsigned char index);

			double getClockingDegree_degs() const { return mClockingDegree_degs; }
			double getAngleFromVertical_stage() const { return mAngleFromVertical_stage; }
			double getLength() const { return mCurrentLength; }
			bool isFullyExtended() const { return mCurrentLength >= mMaxLength; }
			double getForceMagnitude() const { return mSpring.getForce(); }
			const std::vector<PistonCylinder>& getCylinders() const { return mCylinders; }
			glm::dvec3 getMountPoint_stage() const { return mMountPoint_stage; }

		private:
			void update(double newLength, double dt);
			void clampLength();

		};
	}
}

#endif
