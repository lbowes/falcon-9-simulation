#include "Simulation.h"

#include <iomanip>

namespace Physics {

	std::ofstream Simulation::mTextOutput;

	Simulation::Simulation(const std::string& textOutputFilePath) :
		mTextOuputFilePath(textOutputFilePath)
	{
		glm::dvec3 vehiclePosition = { 0.0, 3.0, 0.0 };
		glm::dmat4 vehicleRotation = glm::rotate(glm::radians(0.0), glm::dvec3(1.0, 0.0, 0.0));

		mFalcon9.getState().setObjectToParentTransform({ vehiclePosition, vehicleRotation });

		//Open telemetry text output file
		mTextOutput = std::ofstream(textOutputFilePath);
		if(!mTextOutput)
			printf("Error opening text output file: '%s'\n", textOutputFilePath.c_str());
		else
			mTextOutput << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10 + 2);
	}
	
	void Simulation::run() {
		printf("Started simulation...\n");

		unsigned epoch = 0;
		while (!stopCondMet()) {
			mFalcon9.update(static_cast<double>(mCurrentTime_s), static_cast<double>(mUpdateStepSize_s));
			
			mCurrentTime_s += mUpdateStepSize_s;
			
			if(mCurrentTime_s - mLastSnapshotTime_s >= mSnapshotInterval_s) {
				saveAllDynamicState(mCurrentTime_s, mFalcon9);
				mLastSnapshotTime_s = mCurrentTime_s;
			}

			printf("\rEpoch %i...", epoch);
    		fflush(stdout);
			epoch++;
		}

		printf("\nDone.\n");
	}
	
	bool Simulation::stopCondMet() {
		return mCurrentTime_s >= 10.0;
	}

	void Simulation::saveAllDynamicState(const double currentTime_s, const Hardware::Falcon9& falcon9) {
		//Create a simulation state snapshot for this timestep and populate it
		DSS snapshot;

		snapshot.simulationTime = currentTime_s;

		falcon9.saveDynamicState(snapshot.F9);

		mStateHistory.insert({ currentTime_s, snapshot });
		mTextOutput << "\n";
	}

}