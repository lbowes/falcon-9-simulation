#include "Simulation.h"

namespace Physics {

	Simulation::Simulation() :
		mDuration(5.0),        // 20.0 - todo: should eventually be removed in favour of simulation-termination condition checking
		mUpdatesPerSec(1000),  // 1000
		mDataSnapsPerSec(10),  // 10
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
	}

	void Simulation::run() {
		unsigned short 
			updateCount = 0,
			snapshotCount = 0;

		printf("Simulation started...\n");

		while(!terminationCondMet()) {
			const double dt = 1.0 / mUpdatesPerSec;
			
			mFalcon9.update(dt);
			mSystem.DoStepDynamics(dt);

			if((updateCount + 1) % (mUpdatesPerSec / mDataSnapsPerSec) == 0)
				serialiseSnapshot(snapshotCount++);

			printASCIIProgressBar(mSystem.GetChTime() / mDuration);
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

	void Simulation::printASCIIProgressBar(double progress_0_1) {
		// The maximum width of the progress bar in characters
		const unsigned char maxWidth = 200;

		std::string bar = std::string(maxWidth, '.');

		for(unsigned char i = 0; i < static_cast<size_t>(progress_0_1 * maxWidth); i++)
		    bar[i] = '=';

		bar += "|";

        // Set bold green colour
        printf("\033[1;32m");
		printf("\rTime: %.2f / %.2f s (%.2f %%): %s", mSystem.GetChTime(), mDuration, progress_0_1 * 100.0f, bar.c_str());
		fflush(stdout);
		// Reset to default colour
		printf("\033[0m");
	}

}