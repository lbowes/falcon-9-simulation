#ifndef PHYSICS_HARDWARE_ACCELEROMETER_H
#define PHYSICS_HARDWARE_ACCELEROMETER_H
#pragma once

#include "Sensor.h"

//TODO: Yet to be implemented

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
