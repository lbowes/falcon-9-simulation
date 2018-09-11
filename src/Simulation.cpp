#include "Simulation.h"

#include <iomanip>

namespace Physics {

	const std::string Simulation::mDataOuputFile = "bin/telemetry.dat";
	std::ofstream Simulation::mDataOutput;

	Simulation::Simulation() {
		glm::dvec3 vehiclePosition = { 0.0, 3.0, 0.0 };
		glm::dmat4 vehicleRotation = glm::rotate(glm::radians(0.0), glm::dvec3(1.0, 0.0, 0.0));

		mFalcon9.mutableState().setObjectToParentTransform({ vehiclePosition, vehicleRotation });

		//Open telemetry output file
		mDataOutput = std::ofstream(mDataOuputFile);
		
		if(!mDataOutput)
			printf("Error opening data output file: '%s'\n", mDataOuputFile.c_str());

		mDataOutput << std::fixed << std::setprecision(4);
	}
	
	void Simulation::run() {
		printf("Press enter to begin simulation\n");
		printf("Simulation STARTED...\n");
		std::cin.get();


		while (!stopCondMet()) {
			//Perfom one state update
			mFalcon9.update(static_cast<double>(mCurrentTime_s), static_cast<double>(mUpdateStepSize_s));

			//Update the current time after this update
			mCurrentTime_s += mUpdateStepSize_s;
			
			//If the time since the last data snapshot was taken is greater than the interval, take a snapshot and reset.
			if(mCurrentTime_s - mLastSnapshotTime_s >= mSnapshotInterval_s) {
				outputAll(mCurrentTime_s, mFalcon9);
				mLastSnapshotTime_s = mCurrentTime_s;
			}
		}


		printf("Simulation ENDED.\n");
	}
	
	bool Simulation::stopCondMet() {
		return mCurrentTime_s >= 10.0;
	}

	void Simulation::outputAll(const double currentTime_s, const Hardware::Falcon9& falcon9) {
		//mDataOutput << "testing this stuff at " << std::fixed << std::setprecision(4) << currentTime_s << "\n";

		output(currentTime_s);                             //Snapshot time

		//Stage 1
		{
			//Basic physics state
			const State& s1 = mFalcon9.getStage1().getState();
			output(s1.getCMPosition_world());              //position
			output(s1.getVelocity_world());                //velocity
			output(mFalcon9.getStage1().getAccel_world()); //acceleration
			output(s1.getMomentum_world());                //momentum
			output(s1.getOrientation_world());             //orientation
			output(s1.getAngularVelocity_world());         //angular velocity
			output(s1.getAngularMomentum_world());         //angular momentum
			output(s1.getMass_local().getValue());         //mass

			//
		}



		mDataOutput << "\n";
	}

	void Simulation::output(const glm::dvec3& vec, const char delim) {
		mDataOutput << vec.x << delim;
		mDataOutput << vec.y << delim;
		mDataOutput << vec.z << delim;
	}

	void Simulation::output(const glm::dquat& quat, const char delim) {
		mDataOutput << quat.x << delim;
		mDataOutput << quat.y << delim;
		mDataOutput << quat.z << delim;
		mDataOutput << quat.w << delim;
	}

	void Simulation::output(const double& scalar, const char delim) {
		mDataOutput << scalar << delim;
	}

}