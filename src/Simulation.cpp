#include "Simulation.h"

#include <iomanip>

namespace Physics {

	std::ofstream Simulation::mTextOutput;

	Simulation::Simulation(const std::string& textOutputFilePath) :
		mTextOuputFilePath(textOutputFilePath)
	{
		setupInitialVehicleState();

		//Open telemetry text output file
		mTextOutput = std::ofstream(textOutputFilePath);
		if(!mTextOutput)
			printf("Error opening text output file: '%s'\n", textOutputFilePath.c_str());
		else
			mTextOutput << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10 + 2);
	}
	
	void Simulation::setupInitialVehicleState() {
		//mFalcon9.separateStages();
		//mFalcon9.getStage2().getState().setPosition_world({ 0.0f, 0.0f, 2000.0f});
//
		//State& s = mFalcon9.getStage1().getState();
//
		//s.setPosition_world({ 0.0, 6'101.0, 360.0 });
		//s.setVelocity_world(glm::rotate(glm::dvec3(0.0, 367.0, 0.0), glm::radians(-172.93), glm::dvec3(1.0, 0.0, 0.0)));
		//s.setOrientation_world(glm::toQuat(glm::rotate(glm::radians(7.07), glm::dvec3(1.0, 0.0, 0.0))));
//
		//Hardware::FluidTank 
		//	*s1Lox = mFalcon9.getStage1().getPropellantSupplies().getComponent<Hardware::FluidTank>(Propellants::liquidOxygen),
		//	*s1RP1 = mFalcon9.getStage1().getPropellantSupplies().getComponent<Hardware::FluidTank>(Propellants::RP1);
		//
		//const double scale = 0.90748;
		//s1Lox->removeAllFluid();
		//s1Lox->addFluid(9628.19704433 * scale);
		//s1RP1->removeAllFluid();
		//s1RP1->addFluid(4139.29064039 * scale);
	}

	void Simulation::run() {
		printf("Started simulation...\n");

		unsigned 
			epoch = 0,
			snapshotNumber = 0;

		while (!stopCondMet()) {
			mFalcon9.update(static_cast<double>(mCurrentTime_s), static_cast<double>(mUpdateStepSize_s));
			
			mCurrentTime_s += mUpdateStepSize_s;
			
			//If a snapshot should be taken...
			if((epoch + 1) % (mUpdatesPerSecond / mDataSnapsPerSecond) == 0)
				saveAllDSSToHistory(snapshotNumber++, mFalcon9);
			
			printf("\rEpoch %i...", epoch);
    		fflush(stdout);
			epoch++;
		}

		printf("\nDone.\n");
	}
	
	bool Simulation::stopCondMet() {
		return mCurrentTime_s >= 100.0; //100.0
	}

	void Simulation::saveAllDSSToHistory(const unsigned snapshotNumber, const Hardware::Falcon9& falcon9) {
		//Create a dynamic simulation state snapshot for this timestep...
		DSS snapshot(snapshotNumber, falcon9);

		//...before saving it in the state history.
		mStateHistory.insert({ snapshotNumber, snapshot });
		mTextOutput << "\n";
	}

}