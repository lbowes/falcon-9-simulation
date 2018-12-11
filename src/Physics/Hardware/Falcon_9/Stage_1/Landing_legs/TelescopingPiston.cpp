#include "TelescopingPiston.h"

namespace Physics {
	namespace Hardware {

		TelescopingPiston::TelescopingPiston(double minLength) :
			mSpring(4000.0, 0.0, 1300.0), //4000.0, 0.0, 1300.0
			mMinLength(minLength)
		{ 
			mSubCylinderLengths.push_back(3.286285711); //0.304285714 * totalLength
			mSubCylinderLengths.push_back(2.283428567); //0.211428571 * totalLength
			mSubCylinderLengths.push_back(2.350285718); //0.217619048 * totalLength
			mSubCylinderLengths.push_back(2.386285715); //0.220952381 * totalLength
			mSubCylinderLengths.push_back(0.493714289); //0.045714286 * totalLength
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