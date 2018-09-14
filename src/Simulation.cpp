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
		printf("Started simulation... ");

		while (!stopCondMet()) {
			mFalcon9.update(static_cast<double>(mCurrentTime_s), static_cast<double>(mUpdateStepSize_s));
			
			mCurrentTime_s += mUpdateStepSize_s;
			
			if(mCurrentTime_s - mLastSnapshotTime_s >= mSnapshotInterval_s) {
				outputAll(mCurrentTime_s, mFalcon9);
				mLastSnapshotTime_s = mCurrentTime_s;
			}
		}

		printf("Done.\n");
	}
	
	bool Simulation::stopCondMet() {
		return mCurrentTime_s >= 10.0;
	}

	void Simulation::outputAll(const double currentTime_s, const Hardware::Falcon9& falcon9) {
		//Create a simulation state snapshot for this timestep and populate it
		DynamicSimState snapshot;

		snapshot.mSimulationTime = currentTime_s;
		output(currentTime_s);

		//Falcon 9
		{
			const State& f9State = falcon9.getState();
			
			//Populate state
			snapshot.F9.RB.CoMPosition_world = f9State.getCMPosition_world();
			snapshot.F9.RB.velocity =          f9State.getVelocity_world();
			snapshot.F9.RB.acceleration =      mFalcon9.getAccel_world();
			snapshot.F9.RB.momentum =          f9State.getMomentum_world();
			snapshot.F9.RB.orientation =       f9State.getOrientation_world();
			snapshot.F9.RB.angularVelocity =   f9State.getAngularVelocity_world();
			snapshot.F9.RB.angularMomentum =   f9State.getAngularMomentum_world();
			snapshot.F9.RB.CoMPosition_local = f9State.getMass_local().getCentre();
			snapshot.F9.RB.mass =              f9State.getMass_local().getValue();
			snapshot.F9.RB.localToWorld.setLocalToParent_translation(snapshot.F9.RB.CoMPosition_world);
			snapshot.F9.RB.localToWorld.setLocalToParent_rotation(snapshot.F9.RB.orientation);

			const CoordTransform3D& EUN_to_ECEF = mFalcon9.getSurfaceLocation().getEUN_to_ECEFTransform();
			snapshot.F9.EUN_to_ECEFTransform.setLocalToParent_translation(EUN_to_ECEF.getLocalToParent_translation());
			snapshot.F9.EUN_to_ECEFTransform.setLocalToParent_translation(EUN_to_ECEF.getLocalToParent_rotation());

			//Output to file
			outputRBState(snapshot.F9.RB);

			//Stage 1
			{
				const Hardware::IStage& s1 = mFalcon9.getStage1();
				const State& s1State = s1.getState();

				//Populate state
				snapshot.F9.S1.RB.CoMPosition_world = s1State.getCMPosition_world();
				snapshot.F9.S1.RB.velocity =          s1State.getVelocity_world();
				snapshot.F9.S1.RB.acceleration =      mFalcon9.getStage1().getAccel_world();
				snapshot.F9.S1.RB.momentum =          s1State.getMomentum_world();
				snapshot.F9.S1.RB.orientation =       s1State.getOrientation_world();
				snapshot.F9.S1.RB.angularVelocity =   s1State.getAngularVelocity_world();
				snapshot.F9.S1.RB.angularMomentum =   s1State.getAngularMomentum_world();
				snapshot.F9.S1.RB.CoMPosition_local = s1State.getMass_local().getCentre();
				snapshot.F9.S1.RB.mass =              s1State.getMass_local().getValue();
				snapshot.F9.S1.RB.localToWorld.setLocalToParent_translation(snapshot.F9.S1.RB.CoMPosition_world);
				snapshot.F9.S1.RB.localToWorld.setLocalToParent_rotation(snapshot.F9.S1.RB.orientation);
				snapshot.F9.S1.LOXMass =              static_cast<Hardware::FluidTank*>(s1.getPropellantSupplies().getComponent(Propellants::liquidOxygen))->getPropMassValue_tank();
				snapshot.F9.S1.RP1Mass =              static_cast<Hardware::FluidTank*>(s1.getPropellantSupplies().getComponent(Propellants::RP1))->getPropMassValue_tank();
				snapshot.F9.S1.nitrogenMass =         static_cast<Hardware::FluidTank*>(s1.getPropellantSupplies().getComponent(Propellants::nitrogen))->getPropMassValue_tank();

				//Output to file
				outputRBState(snapshot.F9.S1.RB);
				output(snapshot.F9.S1.LOXMass);
				output(snapshot.F9.S1.RP1Mass);
				output(snapshot.F9.S1.nitrogenMass);
			}

			//Stage 2
			{
				const Hardware::IStage& s2 = mFalcon9.getStage2();
				const State& s2State = s2.getState();

				//Populate state
				snapshot.F9.S2.RB.CoMPosition_world = s2State.getCMPosition_world();
				snapshot.F9.S2.RB.velocity =          s2State.getVelocity_world();
				snapshot.F9.S2.RB.acceleration =      mFalcon9.getStage1().getAccel_world();
				snapshot.F9.S2.RB.momentum =          s2State.getMomentum_world();
				snapshot.F9.S2.RB.orientation =       s2State.getOrientation_world();
				snapshot.F9.S2.RB.angularVelocity =   s2State.getAngularVelocity_world();
				snapshot.F9.S2.RB.angularMomentum =   s2State.getAngularMomentum_world();
				snapshot.F9.S2.RB.CoMPosition_local = s2State.getMass_local().getCentre();
				snapshot.F9.S2.RB.mass =              s2State.getMass_local().getValue();
				snapshot.F9.S1.RB.localToWorld.setLocalToParent_translation(snapshot.F9.S2.RB.CoMPosition_world);
				snapshot.F9.S1.RB.localToWorld.setLocalToParent_rotation(snapshot.F9.S2.RB.orientation);
				snapshot.F9.S2.LOXMass =              static_cast<Hardware::FluidTank*>(s2.getPropellantSupplies().getComponent(Propellants::liquidOxygen))->getPropMassValue_tank();
				snapshot.F9.S2.RP1Mass =              static_cast<Hardware::FluidTank*>(s2.getPropellantSupplies().getComponent(Propellants::RP1))->getPropMassValue_tank();
				snapshot.F9.S2.nitrogenMass =         static_cast<Hardware::FluidTank*>(s2.getPropellantSupplies().getComponent(Propellants::nitrogen))->getPropMassValue_tank();
		
				//Output to file
				outputRBState(snapshot.F9.S2.RB);
				output(snapshot.F9.S2.LOXMass);
				output(snapshot.F9.S2.RP1Mass);
				output(snapshot.F9.S2.nitrogenMass);
			}
		}

		//TODO:
		//Complete the telemetry output with all data coming from the simulation.

		mStateHistory.insert({ currentTime_s, snapshot });
		mTextOutput << "\n";
	}

	void Simulation::outputRBState(const RigidBodyState& RB) {
		output(RB.CoMPosition_world);
		output(RB.velocity);
		output(RB.acceleration);
		output(RB.momentum);
		output(RB.orientation);
		output(RB.angularVelocity);
		output(RB.angularMomentum);
		output(RB.CoMPosition_local);
		output(RB.mass);
	}

	void Simulation::output(const glm::dvec3& vec, const char delim) {
		mTextOutput << vec.x << delim;
		mTextOutput << vec.y << delim;
		mTextOutput << vec.z << delim;
	}

	void Simulation::output(const glm::dquat& quat, const char delim) {
		mTextOutput << quat.x << delim;
		mTextOutput << quat.y << delim;
		mTextOutput << quat.z << delim;
		mTextOutput << quat.w << delim;
	}

	void Simulation::output(const double& scalar, const char delim) {
		mTextOutput << scalar << delim;
	}

}