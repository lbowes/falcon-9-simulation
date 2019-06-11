#ifndef PHYSICS_SIMULATION_H
#define PHYSICS_SIMULATION_H
#pragma once

#include "Internal/Hardware/Falcon_9/F9.h"
#include "External/GroundPlane.h"

#include <chrono/physics/ChSystemNSC.h>
#include <map>

namespace Physics {

	class Simulation {
	private:
		const double mDuration;

		const unsigned short 
			mUpdatesPerSec,
			mDataSnapsPerSec;

		chrono::ChSystemNSC mSystem;
		Hardware::F9 mFalcon9;
		External::GroundPlane mGround;

		std::map<const unsigned, const F9_DSS> mStateHistory;
		
	public:
		Simulation();
		~Simulation() = default;

		void run();
		const std::map<const unsigned, const F9_DSS>& getStateHistory() const { return mStateHistory; }
		double getSnapshotInterval_s() const { return 1.0 / mDataSnapsPerSec; }
		double getDuration() const { return mDuration; }

	private:
		bool terminationCondMet();
		void serialiseSnapshot(unsigned long snapshotNumber);
		void printProgress(double startTime_s, double progress_0_1);

	};

}

#endif // PHYSICS_SIMULATION_H
