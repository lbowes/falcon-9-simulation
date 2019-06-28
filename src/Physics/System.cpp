#include "System.h"

namespace Physics {

    System::System() :
        mFalcon9(mSystem),
        mGround(mSystem)
    {
        chrono::collision::ChCollisionModel::SetDefaultSuggestedEnvelope(1.0);
		chrono::collision::ChCollisionModel::SetDefaultSuggestedMargin(0.05);
		
		mSystem.SetSolverType(chrono::ChSolver::Type::BARZILAIBORWEIN);
		mSystem.SetTimestepperType(chrono::ChTimestepper::Type::EULER_IMPLICIT_PROJECTED);
		mSystem.SetSolverWarmStarting(true);
		mSystem.SetMaxItersSolverSpeed(200); // 200
        //mSystem.Set_G_acc({0, 0, 0});
    }

    void System::update(double dt) {
        // TODO
        // Environment::update(dt);
        // mGround.update(dt);
        mFalcon9.update(dt);
        
        mSystem.DoStepDynamics(dt);
    }

    std::string System::sample() const 
        // Should return one row, to fill the output CSV file
    {
        std::string row;
        
        // TODO: other big objects to collect data from
        // Environment::outputToCSV(row)
        // mGround.outputToCSV(row)
        mFalcon9.outputToCSV(row);

        return row + "\n";
    }

}