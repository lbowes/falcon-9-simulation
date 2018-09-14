#include "TelescopingPiston.h"

namespace Physics {
	namespace Hardware {

		TelescopingPiston::TelescopingPiston(glm::dvec3 mountPoint_stage, double clockingDegree_degs, double minimumLength) :
			mSpring(4000.0, 0.0, 1300.0), //4000.0, 0.0, 1300.0
			mMountPoint_stage(mountPoint_stage),
			mClockingDegree_degs(clockingDegree_degs),
			mMinLength(minimumLength)
		{ 
			for (unsigned char i = 0; i < mCylinderCount; i++)
				mCylinders.push_back(PistonCylinder(*this, mMaxWidth - (mMaxWidth - mMinWidth) * static_cast<float>(i) / 4, i));
		}

		PistonCylinder* TelescopingPiston::getCylinder(unsigned char index) {
			if (index < mCylinders.size())
				return &mCylinders[index];
			else
				return nullptr;
		}

		void TelescopingPiston::update(double newLength, double dt) {
			mLastLength = mCurrentLength;
			mCurrentLength = newLength;
			clampLength();

			mExtensionRate = dt == 0.0 ? 0.0 : (mCurrentLength - mLastLength) / dt;
																	  
			mSpring.update(std::max(mCurrentLength - (mMaxLength - 1.2), 0.0), mExtensionRate);
		}

		void TelescopingPiston::clampLength() {
			if (mCurrentLength < mMinLength)
				mCurrentLength = mMinLength;
			else if (mCurrentLength > mMaxLength)
				mCurrentLength = mMaxLength;
		}

	}
}