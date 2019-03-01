#include "Simulation.h"

//temp
#include <core/ChQuaternion.h>
//

namespace Physics {

	Simulation::Simulation() :
		mDuration(50.0),
		mUpdatesPerSec(250),
		mDataSnapsPerSec(10),
		mFalcon9(mSystem)
	{ 
		chrono::collision::ChCollisionModel::SetDefaultSuggestedEnvelope(1.0);
		chrono::collision::ChCollisionModel::SetDefaultSuggestedMargin(0.05);
		
		// temp
		mSystem.SetSolverType(chrono::ChSolver::Type::BARZILAIBORWEIN);
		//mSystem.SetTimestepperType(chrono::ChTimestepper::Type::EULER_IMPLICIT_LINEARIZED);
		//mSystem.SetMaxPenetrationRecoverySpeed(3);
		//mSystem.SetMaxItersSolverSpeed(15);

		// FLOOR
		floor = std::make_shared<chrono::ChBody>();
    	mSystem.Add(floor);

		floor->GetCollisionModel()->SetEnvelope(2.0);
		floor->GetCollisionModel()->SetSafeMargin(2.0);
		floor->GetCollisionModel()->ClearModel();
		floor->GetCollisionModel()->AddBox(25, 3, 25);
		floor->GetCollisionModel()->BuildModel();
		floor->SetCollide(true);
		floor->SetMass(8000);
		floor->SetBodyFixed(true);
		floor->SetPos({0, -3, 0});
		floor->GetMaterialSurfaceNSC()->SetRestitution(0);
		floor->GetMaterialSurfaceNSC()->SetFriction(1);
		floor->GetMaterialSurfaceNSC()->SetFriction(1);
		floor->GetMaterialSurfaceNSC()->SetRollingFriction(1);
		floor->GetMaterialSurfaceNSC()->SetSpinningFriction(1);
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