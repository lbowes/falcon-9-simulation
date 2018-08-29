#ifndef PHYSICS_HARDWARE_ACCELEROMETER_H
#define PHYSICS_HARDWARE_ACCELEROMETER_H
#pragma once

#include "Sensor.h"

namespace Physics {
	namespace Hardware {
		
		class Accelerometer {
		private:
			Sensor linAccelAlongAxis;

		public:
			Accelerometer();
			~Accelerometer() = default;

		};

	}
}

#endif
