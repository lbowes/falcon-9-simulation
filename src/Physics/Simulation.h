#ifndef PHYSICS_SIMULATION_H
#define PHYSICS_SIMULATION_H
#pragma once

#include "chrono/physics/ChSystemNSC.h"
#include "Internal/Hardware/Falcon_9/Falcon9.h"
#include "DynamicSimState.h"

#include <map>

/*
Currently working on finding out how to store the state of the simulation each timestep.
Stuff found so far that might be useful are:
- ChState class
- ChVariablesBody class
*/

namespace Physics {

	class Simulation {
	private:
		const unsigned short 
			mUpdatesPerSec,
			mDataSnapsPerSec;

		chrono::ChSystemNSC mSystem;
		Hardware::Falcon9 mFalcon9;

		std::map<const unsigned, const DSS> mStateHistory;

	public:
		Simulation();
		~Simulation() = default;

		void run();
		const std::map<const unsigned, const DSS>& getStateHistory() const { return mStateHistory; }
		double getSnapshotInterval_s() const { return 1.0 / mDataSnapsPerSec; }

	private:
		bool terminationCondMet();
		void serialiseSnapshot(const Hardware::Falcon9& f9s1, unsigned long snapshotNumber);

	};

}

#endif
