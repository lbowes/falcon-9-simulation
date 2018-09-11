#ifndef PHYSICS_SIMULATION_H
#define PHYSICS_SIMULATION_H

#include "Physics/Hardware/Falcon_9/Falcon9.h"

#include <string>
#include <fstream>

namespace Physics {

	class Simulation {
	private:
		static constexpr unsigned short 
			mUpdatesPerSecond = 1000,
			mDataSnapsPerSecond = 4;

		static constexpr long double 
			mUpdateStepSize_s = 1.0 / mUpdatesPerSecond,
			mSnapshotInterval_s = 1.0 / mDataSnapsPerSecond;

		long double 
			mCurrentTime_s = 0.0,
			mLastSnapshotTime_s = 0.0;

		static const std::string mDataOuputFile;
		
		static std::ofstream mDataOutput;

		Physics::Hardware::Falcon9 mFalcon9;

	public:
		Simulation();
		~Simulation() = default;

		void run();

	private:
		void load();
		bool stopCondMet();
		void outputAll(const double currentTime, const Hardware::Falcon9& falcon9);
		void output(const glm::dvec3& vec, const char delim = ',');
		void output(const glm::dquat& quat, const char delim = ',');
		void output(const double& scalar, const char delim = ',');

	};

}

#endif
