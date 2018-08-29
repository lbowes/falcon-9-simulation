#ifndef PHYSICS_HARDWARE_GYROSCOPE_H
#define PHYSICS_HARDWARE_GYROSCOPE_H
#pragma once

#include "Sensor.h"

namespace Physics {
	namespace Hardware {
		
		class Gyroscope {
		private:
			Sensor angularAccelAlongAxis;

		public:
			Gyroscope();
			~Gyroscope() = default;

		};

	}
}

#endif
