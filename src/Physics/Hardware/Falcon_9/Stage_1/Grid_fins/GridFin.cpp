#include "GridFin.h"

#include <iostream>

namespace Physics {
	namespace Hardware {

		GridFin::GridFin(double clockingDegree_degs) :
			IStageComponent(CoordTransform3D(), 300.0),
			mClockingDegree_degs(clockingDegree_degs)
		{
			mCompToStage.setLocalToParent_position(rotate(glm::dvec3(0.0, m2DMountPoint_stage.y, -m2DMountPoint_stage.x), glm::radians(clockingDegree_degs), glm::dvec3(0.0, 1.0, 0.0)));
			updateComponentSpace3D();
			mMass_comp.setCentre(glm::dvec3(0.0, 0.0, -mCmDistFromTop));
		}

		void GridFin::update(double dt/* , double fluidDensity, glm::dvec3 flowVelocity_stage */) {
			switch (mCurrentPhase) {
				case Phase::stowed:
					mPercentDeployed = 0.0;
					break;

				case Phase::deploying: {
					mPercentDeployed += mDeployRate * dt;

					if (mPercentDeployed > 1.0) {
						mPercentDeployed = 1.0;
						mCurrentPhase = Phase::deployed;
					}
					else if (mPercentDeployed < 0.0)
						mPercentDeployed = 0.0;

					break;
				}

				case Phase::deployed:
					mPercentDeployed = 1.0;
					break;

				case Phase::stowing: {
					if (abs(mRollAngle) > 0.1) {
						mRollAngle -= mRollAngle * 0.95 * dt;
						return;
					}
					else
						mPercentDeployed -= mStowRate * dt;

					if (mPercentDeployed <= 0.0) {
						mPercentDeployed = 0.0;
						mCurrentPhase = Phase::stowed;
					}
					break;
				}

				default:
					break;
			}

			mPitchAngle = -90.0 + (mPercentDeployed * 90.0);

			updateComponentSpace3D();
		}

		void GridFin::setRoll(double newRollValue) {
			bool finCanRotate = (mCurrentPhase == Phase::deployed && newRollValue >= -(mRollRange_degs * 0.5) && newRollValue <= (mRollRange_degs * 0.5));
			if (finCanRotate)
				mRollAngle = newRollValue;
		}

		void GridFin::deploy() {
			if (mCurrentPhase == Phase::stowed)
				mCurrentPhase = Phase::deploying;
		}

		void GridFin::stow() {
			if (mCurrentPhase == Phase::deployed)
				mCurrentPhase = Phase::stowing;
		}

		void GridFin::updateComponentSpace3D() {
			using namespace glm;

			dmat4 
				roll = rotate(radians(mRollAngle), dvec3(0.0, 0.0, -1.0)),
				pitch = rotate(radians(mPitchAngle), dvec3(1.0, 0.0, 0.0)),
				yaw = rotate(radians(mClockingDegree_degs), dvec3(0.0, 1.0, 0.0));
			
			mCompToStage.setLocalToParent_rotation(yaw * roll * pitch);
		}

	}
}