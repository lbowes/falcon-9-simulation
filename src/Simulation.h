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
			mUpdatesPerSecond = 60, //200
			mDataSnapsPerSecond = 60;

		static constexpr double mUpdateStepSize_s = 1.0 / mUpdatesPerSecond;

		double mCurrentTime_s = 0.0;

		const std::string mTextOuputFilePath;
		
		static std::ofstream mTextOutput;

		Physics::Hardware::Falcon9 mFalcon9;

		std::map<const unsigned, const DSS> mStateHistory;

	public:
		Simulation(const std::string& textOutputFilePath);
		~Simulation() = default;

		void run();
		const std::map<const unsigned, const DSS>& getStateHistory() const { return mStateHistory; }
		double getSnapshotInterval() const { return 1.0 / mDataSnapsPerSecond; }

	private:
		//temp
		void setupInitialVehicleState();
		//
		
		void load();
		bool stopCondMet();
		void saveAllDSSToHistory(const unsigned snapshotNumber, const Hardware::Falcon9& falcon9);
		void output(const glm::dvec3& vec, const char delim = ',');
		void output(const glm::dquat& quat, const char delim = ',');
		void output(const double& scalar, const char delim = ',');

	};

}

#endif
