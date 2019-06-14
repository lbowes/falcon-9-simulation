#include "Simulation.h"
#include <chrono>

namespace Physics {

	Simulation::Simulation() :
		mDuration(10.0),       // 20.0 - todo: should eventually be removed in favour of simulation-termination condition checking
		mUpdatesPerSec(100),  // 1000
		mDataSnapsPerSec(10), // 10 - sample the state of the simulation every 1/10th of a second
		mFalcon9(mSystem),
		mGround(mSystem)
	{ 
		chrono::collision::ChCollisionModel::SetDefaultSuggestedEnvelope(1.0);
		chrono::collision::ChCollisionModel::SetDefaultSuggestedMargin(0.05);
		
		// temp
		mSystem.SetSolverType(chrono::ChSolver::Type::BARZILAIBORWEIN);
		mSystem.SetTimestepperType(chrono::ChTimestepper::Type::EULER_IMPLICIT_PROJECTED);
		mSystem.SetSolverWarmStarting(true);
		mSystem.SetMaxItersSolverSpeed(200); // 200
		//
        
        //mSystem.Set_G_acc({0, 0, 0});
	}

	void Simulation::run() {
		unsigned short 
			updateCount = 0,
			snapshotCount = 0;

		printf("Simulation started...\n");

        const auto startTime = std::chrono::high_resolution_clock::now();

		while(!terminationCondMet()) {
			const double dt = 1.0 / mUpdatesPerSec;
			
			mFalcon9.update(dt);
			mSystem.DoStepDynamics(dt);

			if((updateCount + 1) % (mUpdatesPerSec / mDataSnapsPerSec) == 0)
				serialiseSnapshot(snapshotCount++);

            const auto currentTime = std::chrono::high_resolution_clock::now();
			const double timeTaken_s = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0;
            printProgress(timeTaken_s, mSystem.GetChTime() / mDuration);
			updateCount++;
		}

		printf("\nSimulation terminated.\n");
	}

	bool Simulation::terminationCondMet() {
		return mSystem.GetChTime() >= mDuration;
	}

	void Simulation::serialiseSnapshot(unsigned long snapshotNumber) {
		F9_DSS snapshot = F9_DSS(mFalcon9);
		mStateHistory.insert({snapshotNumber, snapshot});

		// TODO: Consider outputting the entire state of the simulation to a file.
		// How large would this be if all state was saved at 1000 Hz?
		// This would then be useful for reloading the visualisation quickly. A check would be required to
		// see if the simulation had changed and therefore whether or not it would have to be re-run.
	}

	void Simulation::printProgress(double timeTaken_s, double progress_0_1) {
		// The maximum width of the progress bar in characters
        // Set bold green colour
        printf("\033[1;32m");
		printf("\r%.2f s Simulated: %.2f / %.2f s (%.2f %%)", timeTaken_s, mSystem.GetChTime(), mDuration, progress_0_1 * 100.0f);
		fflush(stdout);
		// Reset to default colour
		printf("\033[0m");
	}

}