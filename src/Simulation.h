#ifndef PHYSICS_SIMULATION_H
#define PHYSICS_SIMULATION_H

#include "Physics/Hardware/Falcon_9/Falcon9.h"
#include "Physics/DynamicSimState.h"

#include <string>
#include <fstream>
#include <map>

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

		const std::string mTextOuputFilePath;
		
		static std::ofstream mTextOutput;

		Physics::Hardware::Falcon9 mFalcon9;

		std::map<double, DynamicSimState> mStateHistory;

	public:
		Simulation(const std::string& textOutputFilePath);
		~Simulation() = default;

		void run();
		const std::map<double, DynamicSimState>& getStateHistory() const { return mStateHistory; }

	private:
		void load();
		bool stopCondMet();
		void outputAll(const double currentTime, const Hardware::Falcon9& falcon9);
		void outputRBState(const RigidBodyState& RB);
		void output(const glm::dvec3& vec, const char delim = ',');
		void output(const glm::dquat& quat, const char delim = ',');
		void output(const double& scalar, const char delim = ',');

	};

}

#endif
