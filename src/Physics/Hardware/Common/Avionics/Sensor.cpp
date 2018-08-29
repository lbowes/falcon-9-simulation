#include "Sensor.h"

namespace Physics {
	namespace Hardware {
	
		Sensor::Sensor(const double& trueValue, float errorRange) :
			mTrueValue(trueValue),
			mErrorRange(errorRange)
		{ }

		void Sensor::updateReading() {
			mReading = mTrueValue + ((rand() / RAND_MAX) * 2 - 1) * mErrorRange;
		}

	}
}