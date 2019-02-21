#include "Simulation.h"

//temp
#include <core/ChQuaternion.h>
//

namespace Physics {

	Simulation::Simulation() :
		mDuration(50.0),
		mUpdatesPerSec(100),
		mDataSnapsPerSec(60),
		mFalcon9(mSystem)
	{ 
		chrono::collision::ChCollisionModel::SetDefaultSuggestedEnvelope(1.0);
		chrono::collision::ChCollisionModel::SetDefaultSuggestedMargin(0.05);
		
		// temp
		mSystem.SetSolverType(chrono::ChSolver::Type::APGD);
		mSystem.SetMaxPenetrationRecoverySpeed(3);
		mSystem.SetMaxItersSolverSpeed(30);

		// FLOOR
		rigidFloor = std::make_shared<chrono::ChBody>();
    	mSystem.Add(rigidFloor);

		rigidFloor->GetCollisionModel()->ClearModel();
		rigidFloor->GetCollisionModel()->AddBox(25, 3, 25);
		rigidFloor->GetCollisionModel()->BuildModel();
		rigidFloor->SetCollide(true);
		rigidFloor->SetBodyFixed(true);
		rigidFloor->SetPos({0, -3, 0});
		rigidFloor->SetMass(1000.0);
		rigidFloor->GetMaterialSurfaceNSC()->SetRestitution(0);
		rigidFloor->GetMaterialSurfaceNSC()->SetFriction(1);
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

			printf("\rTime: %.2f", mSystem.GetChTime());
			fflush(stdout);
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
	}

}