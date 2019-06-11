#ifndef PHYSICS_HARDWARE_ILAUNCHVEHICLE_H
#define PHYSICS_HARDWARE_ILAUNCHVEHICLE_H
#pragma once

#include <vector>
#include <core/ChFrame.h>

namespace chrono {
	class ChSystemNSC;
}

namespace Physics {
	namespace Hardware {

		class IStage;

		class ILaunchVehicle {
		private:
			chrono::ChSystemNSC& mSystemHandle;
			std::vector<IStage*> mStages;

		public:
			ILaunchVehicle(chrono::ChSystemNSC& sys);
			virtual ~ILaunchVehicle() = default;

			void update(double dt);

		protected:
			void addStage(IStage& s, const chrono::ChFrame<>& LV_to_Stage = chrono::ChFrame<>());

			virtual void assemble() = 0;

		};

	}
}

#endif // PHYSICS_HARDWARE_ILAUNCHVEHICLE_H