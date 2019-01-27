#include "Simulation.h"

namespace Physics {

	Simulation::Simulation() :
		mFalcon9(mSystem),
		mUpdatesPerSec(60),
		mDataSnapsPerSec(60)
	{ }

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

			printf("\rTime: %.2f", mSystem.GetChTime());
			fflush(stdout);
			updateCount++;
		}

		printf("\nSimulation terminated.\n");
	}

	bool Simulation::terminationCondMet() {
		return mSystem.GetChTime() >= 10;
	}

	void Simulation::serialiseSnapshot(unsigned long snapshotNumber) {
		F9_DSS snapshot = F9_DSS(mFalcon9);
		mStateHistory.insert({snapshotNumber, snapshot});
	}

}