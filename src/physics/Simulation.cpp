#include "Simulation.h"

#include <chrono>

namespace Physics {

	Simulation::Simulation(const std::string& outputCSVFilepath) :
		mDuration_s(10),    // 20.0 - todo: should eventually be removed in favour of simulation-termination condition checking
		mUpdatesFreq(100), // 1000
        mSampleFreq(10),   // 10 - sample the state of the simulation every 1/10th of a second
		mOuptutCSVFilepath(outputCSVFilepath),
        mOutputCSVFile(outputCSVFilepath),
        mSimulatedTime_s(0)
	{ 
        // todo: Look at whether or not the simulation actually needs to be re-run everytime the application runs
        init();
        run();
        close();
	}

    void Simulation::init() {
        mOutputCSVFile << "S1_RP1Tank_pos_x,S1_RP1Tank_pos_y,S1_RP1Tank_pos_z,S1_RP1Tank_rot_e0,S1_RP1Tank_rot_e1,S1_RP1Tank_rot_e2,S1_RP1Tank_rot_e3\n";
    }

	void Simulation::run() {
		unsigned short updateCount = 0;

		printf("Simulation started...\n");

        const auto startTime_s = std::chrono::high_resolution_clock::now();

		while(!terminateCondMet()) {
            const double dt = 1.0 / mUpdatesFreq;
            
            // |=============|
            // | MAIN UPDATE |
            mSystem.update(dt);
            // |=============|

            mSimulatedTime_s += dt;

			if((updateCount + 1) % (mUpdatesFreq / mSampleFreq) == 0)
				serialiseSample();

            const auto currentTime_s = std::chrono::high_resolution_clock::now();
			const double timeTaken_s = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime_s - startTime_s).count() / 1000.0;
            printProgress(timeTaken_s, mSimulatedTime_s / mDuration_s);
			updateCount++;
		}

		printf("\nSimulation terminated.\n");
	}

    void Simulation::close() {
        mOutputCSVFile.close();
    }

	bool Simulation::terminateCondMet() {
		return mSimulatedTime_s >= mDuration_s;
	}

	void Simulation::serialiseSample() {
        mOutputCSVFile << mSystem.sample();            
	}

	void Simulation::printProgress(double timeTaken_s, double progress_0_1) {
        // Bold green colour
        printf("\033[1;32m");

		printf("\r%.2f s Simulated: %.2f / %.2f s (%.2f %%)", timeTaken_s, mSimulatedTime_s, mDuration_s, progress_0_1 * 100.0f);
		fflush(stdout);

		// Default colour
		printf("\033[0m");
	}

}