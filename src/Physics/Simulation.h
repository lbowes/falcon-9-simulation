#ifndef PHYSICS_SIMULATION_H
#define PHYSICS_SIMULATION_H
#pragma once

#include "System.h"

namespace Physics {

	class Simulation {
	private:
		const double mDuration_s;

		const unsigned short 
			mUpdatesFreq,
			mSampleFreq;

        const std::string mOuptutCSVFile;

        double mSimulatedTime_s;

        System mSystem;

	public:
		Simulation(const std::string& outputCSVFilepath);
		~Simulation() = default;

		double getSampleInterval_s() const { return 1.0 / mSampleFreq; }
		double getDuration_s() const { return mDuration_s; }

	private:
		void run();
		bool terminateCondMet();
		void serialiseSample(unsigned long snapshotNumber);
		void printProgress(double startTime_s, double progress_0_1);

	};

}

#endif // PHYSICS_SIMULATION_H
