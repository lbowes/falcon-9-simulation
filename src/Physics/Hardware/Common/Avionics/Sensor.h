#ifndef PHYSICS_HARDWARE_SENSOR_H
#define PHYSICS_HARDWARE_SENSOR_H
#pragma once

#include <cstdlib>

//TODO: Yet to be implemented

namespace Physics {
	namespace Hardware {
		
		class Sensor {
		protected:
			const double& mTrueValue;
			double mReading = 0.0;
			const float mErrorRange = 0.0f;
			
			//TODO: Add other params in here that describe the error/uncertainty/chance of failure etc of the sensor

		public:
			Sensor(const double& trueValue, float errorRange = 0.0f);
			~Sensor() = default;

			void updateReading();

			double getReading() const { return mReading; }

		};

	}
}

#endif
