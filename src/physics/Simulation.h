#ifndef F9SIM_PHYSICS_SIMULATION_H_
#define F9SIM_PHYSICS_SIMULATION_H_
#pragma once

#include "System.h"

namespace Physics {

	class Simulation {
	private:
		const double mDuration_s;

		const unsigned short
			mUpdatesFreq,
			mSampleFreq;

        const std::string mOuptutCSVFilepath;
        std::ofstream mOutputCSVFile;

        double mSimulatedTime_s;

        System mSystem;

	public:
		Simulation(const std::string& outputCSVFilepath);
		~Simulation() = default;

		double getSampleInterval_s() const { return 1.0 / mSampleFreq; }
		double getDuration_s() const { return mDuration_s; }

	private:
        void init();
		void run();
        void close();
		bool terminateCondMet();
		void serialiseSample();
		void printProgress(double startTime_s, double progress_0_1);

	};

}

#endif // F9SIM_PHYSICS_SIMULATION_H_
