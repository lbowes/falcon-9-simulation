#include "Simulation.h"

#include <iomanip>

namespace Physics {

	std::ofstream Simulation::mTextOutput;

	Simulation::Simulation(const std::string& textOutputFilePath) :
		mTextOuputFilePath(textOutputFilePath)
	{
		glm::dvec3 vehiclePosition = { 0.0, 0.0, 0.0 };
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

		unsigned 
			epoch = 0,
			snapshotNumber = 0;

		while (!stopCondMet()) {
			mFalcon9.update(static_cast<double>(mCurrentTime_s), static_cast<double>(mUpdateStepSize_s));
			
			mCurrentTime_s += mUpdateStepSize_s;
			
			//If a snapshot should be taken...
			if((epoch + 1) % (mUpdatesPerSecond / mDataSnapsPerSecond) == 0)
				saveAllDynamicState(snapshotNumber++, mFalcon9);
			
			printf("\rEpoch %i...", epoch);
    		fflush(stdout);
			epoch++;
		}

		printf("\nDone.\n");
	}
	
	bool Simulation::stopCondMet() {
		return mCurrentTime_s >= 100.0;
	}

	void Simulation::saveAllDynamicState(const unsigned snapshotNumber, const Hardware::Falcon9& falcon9) {
		//Create a dynamic simulation state snapshot for this timestep...
		DSS snapshot;

		//...and populate it...
		snapshot.number = snapshotNumber;
		falcon9.saveDynamicState(snapshot.F9);

		//...before saving it in the state history.
		mStateHistory.insert({ snapshotNumber, snapshot });
		mTextOutput << "\n";
	}

}