#include "DynamicSimState.h"
#include "Physics/Hardware/Falcon_9/Falcon9.h"

namespace Physics {

	double lerp(const double a, const double b, const double x) {
		return a + (b - a) * x;
	}

	DSS::DSS(const std::string& dataString) 
		//Parses data string to fully populate the member state
	{
		//Parse the data string into a vector of items
		std::vector<double> dataPoints;
		std::stringstream ss(dataString);
		double d;

		while(ss >> d) {
			dataPoints.push_back(d);

			if(ss.peek() == ',')
				ss.ignore();
		}

		//TODO: Fully initialise the dynamic state using the string passed in	
	}

	DSS::DSS(const unsigned snapshotNumber, const Hardware::Falcon9& falcon9) {
		using namespace Hardware;

		number = snapshotNumber;

		//Falcon 9
		{
			RigidBodyStateSnapshot::save(falcon9, F9.RB);

			//Stage 1
			{
				RigidBodyStateSnapshot::save(falcon9.mStage1, F9.S1.RB);

				//Propellant tanks
				const FluidTankGroup& s1Props = falcon9.mStage1.getPropellantSupplies();
				saveFluidTankState(*s1Props.getComponent<FluidTank>(Propellants::liquidOxygen), F9.S1.LOXTank);
				saveFluidTankState(*s1Props.getComponent<FluidTank>(Propellants::RP1), F9.S1.RP1Tank);
				saveFluidTankState(*s1Props.getComponent<FluidTank>(Propellants::nitrogen), F9.S1.nitrogenTank);

				//Engines
				const ThrustGeneratorGroup& octawebEngines = falcon9.mStage1.getEngines();
				for(unsigned char i = 0; i < octawebEngines.getCount(); i++)
					saveEngineState(*octawebEngines.getComponent<Engine>(i), F9.S1.engines[i]);

				//Gas thrusters
				const ThrustGeneratorGroup& gasThrusters = falcon9.mStage1.getThrusters();
				for(unsigned char i = 0; i < gasThrusters.getCount(); i++)
					saveGasThrusterState(*gasThrusters.getComponent<GasThruster>(i), F9.S1.thrusters[i]);

				//Landing legs
				const LandingLegs& landingLegs = falcon9.mStage1.getLandingLegs();
				for(unsigned char i = 0; i < landingLegs.getCount(); i++)
					saveLandingLegState(*landingLegs.getComponent<LandingLeg>(i), F9.S1.landingLegs[i]);

				//Grid fins
				const GridFins& gridFins = falcon9.mStage1.getGridFins();
				for(unsigned char i = 0; i < gridFins.getCount(); i++)
					saveGridFinState(*gridFins.getComponent<GridFin>(i), F9.S1.gridFins[i]);
			}

			//Stage 2
			{
				RigidBodyStateSnapshot::save(falcon9.mStage2, F9.S2.RB);

				//Propellant tanks
				const FluidTankGroup& s2Props = falcon9.mStage1.getPropellantSupplies();
				saveFluidTankState(*s2Props.getComponent<FluidTank>(Propellants::liquidOxygen), F9.S2.LOXTank);
				saveFluidTankState(*s2Props.getComponent<FluidTank>(Propellants::RP1), F9.S2.RP1Tank);

				//Engines
				saveEngineState(*falcon9.mStage2.getEngines().getComponent<Engine>(0), F9.S2.engine);
			}
		}
	}

	void DSS::load(const DSS& source, Physics::Hardware::Falcon9& dest) {
		using namespace Hardware;

		//Falcon 9
		{
			RigidBodyStateSnapshot::load(source.F9.RB, dest.mState);

			//Stage 1
			{
				RigidBodyStateSnapshot::load(source.F9.S1.RB, dest.mStage1.getState());

				//Propellant tanks
				const FluidTankGroup& s1Props = dest.mStage1.getPropellantSupplies();
				loadFluidTankState(source.F9.S1.LOXTank, *s1Props.getComponent<FluidTank>(Propellants::liquidOxygen));
				loadFluidTankState(source.F9.S1.RP1Tank, *s1Props.getComponent<FluidTank>(Propellants::RP1));
				loadFluidTankState(source.F9.S1.nitrogenTank, *s1Props.getComponent<FluidTank>(Propellants::nitrogen));

				//Engines
				const ThrustGeneratorGroup& octawebEngines = dest.mStage1.getEngines();
				for(unsigned char i = 0; i < octawebEngines.getCount(); i++)
					loadEngineState(source.F9.S1.engines[i], *octawebEngines.getComponent<Engine>(i));

				//Gas thrusters
				const ThrustGeneratorGroup& gasThrusters = dest.mStage1.getThrusters();
				for(unsigned char i = 0; i < gasThrusters.getCount(); i++)
					loadGasThrusterState(source.F9.S1.thrusters[i], *gasThrusters.getComponent<GasThruster>(i));

				//Landing legs
				const LandingLegs& landingLegs = dest.mStage1.getLandingLegs();
				for(unsigned char i = 0; i < landingLegs.getCount(); i++)
					loadLandingLegState(source.F9.S1.landingLegs[i], *landingLegs.getComponent<LandingLeg>(i));

				//Grid fins
				const GridFins& gridFins = dest.mStage1.getGridFins();
				for(unsigned char i = 0; i < gridFins.getCount(); i++)
					loadGridFinState(source.F9.S1.gridFins[i], *gridFins.getComponent<GridFin>(i));
			}

			//Stage 2
			{
				RigidBodyStateSnapshot::load(source.F9.S2.RB, dest.mStage2.getState());

				//Propellant tanks
				const FluidTankGroup& s2Props = dest.mStage1.getPropellantSupplies();
				loadFluidTankState(source.F9.S2.LOXTank, *s2Props.getComponent<FluidTank>(Propellants::liquidOxygen));
				loadFluidTankState(source.F9.S2.RP1Tank, *s2Props.getComponent<FluidTank>(Propellants::RP1));

				//Engines
				loadEngineState(source.F9.S2.engine, *dest.mStage2.getEngines().getComponent<Engine>(0));
			}
		}
	}

	void DSS::lerp(const DSS& a, const DSS& b, double x, DSS& dest) 
		//TODO: linearly interpolate all members of snapshot between 'a' and 'b' and store in 'dest'
	{
		using namespace Hardware;
		
		//Falcon 9
		{
			RigidBodyStateSnapshot::lerp(a.F9.RB, b.F9.RB, x, dest.F9.RB);

			//Stage 1
			{
				RigidBodyStateSnapshot::lerp(a.F9.S1.RB, b.F9.S1.RB, x, dest.F9.S1.RB);

				//Propellant tanks
				lerpFluidTankState(a.F9.S1.LOXTank, b.F9.S1.LOXTank, x, dest.F9.S1.LOXTank);
				lerpFluidTankState(a.F9.S1.RP1Tank, b.F9.S1.RP1Tank, x, dest.F9.S1.RP1Tank);
				lerpFluidTankState(a.F9.S1.nitrogenTank, b.F9.S1.nitrogenTank, x, dest.F9.S1.nitrogenTank);

				//Engines
				for(unsigned char i = 0; i < a.F9.S1.engines.size(); i++)
					lerpEngineState(a.F9.S1.engines[i], b.F9.S1.engines[i], x, dest.F9.S1.engines[i]);

				//Gas thrusters
				for(unsigned char i = 0; i < a.F9.S1.thrusters.size(); i++)
					lerpGasThrusterState(a.F9.S1.thrusters[i], b.F9.S1.thrusters[i], x, dest.F9.S1.thrusters[i]);

				//Landing legs
				for(unsigned char i = 0; i < a.F9.S1.landingLegs.size(); i++)
					lerpLandingLegState(a.F9.S1.landingLegs[i], b.F9.S1.landingLegs[i], x, dest.F9.S1.landingLegs[i]);

				//Grid fins
				for(unsigned char i = 0; i < a.F9.S1.gridFins.size(); i++)
					lerpGridFinState(a.F9.S1.gridFins[i], b.F9.S1.gridFins[i], x, dest.F9.S1.gridFins[i]);
			}

			//Stage 2
			{
				RigidBodyStateSnapshot::lerp(a.F9.S2.RB, b.F9.S2.RB, x, dest.F9.S2.RB);

				//Propellant tanks
				lerpFluidTankState(a.F9.S2.LOXTank, b.F9.S2.LOXTank, x, dest.F9.S2.LOXTank);
				lerpFluidTankState(a.F9.S2.RP1Tank, b.F9.S2.RP1Tank, x, dest.F9.S2.RP1Tank);

				//Engines
				lerpEngineState(a.F9.S2.engine, b.F9.S2.engine, x, dest.F9.S2.engine);
			}
		}	
	}

	void DSS::saveFluidTankState(const Physics::Hardware::FluidTank& source, DSS::FluidTankState& dest) {
		//TODO: Make FluidTank class friends with (only) this function and complete implementation here
		dest.fluidVolume = source.mFluidVolume;
		dest.fluidLevel = source.mFluidLevel;
		dest.percentFull = source.mPercentFull;
		dest.mass_comp = source.mMass_comp;
		dest.fluidMass_tank = source.mFluidMass_tank;
		dest.CoMInertia_comp = source.mCoMInertia_comp;
		dest.fluidInertia_fluidCoM = source.mFluidInertia_fluidCoM;
	}
	
	void DSS::loadFluidTankState(const DSS::FluidTankState& source, Physics::Hardware::FluidTank& dest) {
		dest.mFluidVolume = source.fluidVolume;
		dest.mFluidLevel = source.fluidLevel;
		dest.mPercentFull = source.percentFull;
		dest.mMass_comp = source.mass_comp;
		dest.mFluidMass_tank = source.fluidMass_tank;
		dest.mCoMInertia_comp = source.CoMInertia_comp;
		dest.mFluidInertia_fluidCoM = source.fluidInertia_fluidCoM;
	}

	void DSS::lerpFluidTankState(const DSS::FluidTankState& a, const DSS::FluidTankState& b, double x, DSS::FluidTankState& dest) {
		dest.fluidVolume = Physics::lerp(a.fluidVolume, b.fluidVolume, x);
		dest.fluidLevel = Physics::lerp(a.fluidLevel, b.fluidLevel, x);
		dest.percentFull = Physics::lerp(a.percentFull, b.percentFull, x);
		dest.mass_comp = ::lerp(a.mass_comp, b.mass_comp, x);
		dest.fluidMass_tank = ::lerp(a.fluidMass_tank, b.fluidMass_tank, x);
		dest.fluidVolume = Physics::lerp(a.fluidVolume, b.fluidVolume, x);
		
		//Inertia cannot be interpolated without first interpolating all components contributing to its value.
		//Therefore, the inertia tensor of 'a' is used
		dest.CoMInertia_comp = a.CoMInertia_comp;
		dest.fluidInertia_fluidCoM = a.fluidInertia_fluidCoM;
	}

	void DSS::saveEngineState(const Physics::Hardware::Engine& source, DSS::MerlinEngineState& dest) {
		dest.thrustMagnitude = source.mThrustMagnitude;
		dest.thrust_stage = source.mThrust_stage;
		dest.active = source.mActive;
		dest.throttle = source.mThrottle;
		dest.currentMassFlowRate = source.mCurrentMassFlowRate;
		dest.compToStage = source.mCompToStage;

		for(unsigned char i = 0; i < source.mTVCActuators.size(); i++)
			dest.TVCActuatorAngles[i] = source.mTVCActuators[i].mGimbalAngle;
	}

	void DSS::loadEngineState(const DSS::MerlinEngineState& source, Physics::Hardware::Engine& dest) {
		dest.mThrustMagnitude = source.thrustMagnitude;
		dest.mThrust_stage = source.thrust_stage;
		dest.mActive = source.active;
		dest.mThrottle = source.throttle;
		dest.mCurrentMassFlowRate = source.currentMassFlowRate;
		dest.mCompToStage = source.compToStage;
		
		for(unsigned char i = 0; i < dest.mTVCActuators.size(); i++)
			dest.mTVCActuators[i].mGimbalAngle = source.TVCActuatorAngles[i];
	}

	void DSS::lerpEngineState(const DSS::MerlinEngineState& a, const DSS::MerlinEngineState& b, double x, DSS::MerlinEngineState& dest) {
		dest.thrustMagnitude = Physics::lerp(a.thrustMagnitude, b.thrustMagnitude, x);
		dest.thrust_stage = glm::lerp(a.thrust_stage, b.thrust_stage, x);
		dest.active = a.active;
		dest.throttle = Physics::lerp(a.throttle, b.throttle, x);
		dest.currentMassFlowRate = Physics::lerp(a.currentMassFlowRate, b.currentMassFlowRate, x);
		dest.compToStage = ::lerp(a.compToStage, b.compToStage, x);

		for(unsigned char i = 0; i < a.TVCActuatorAngles.size(); i++)
			dest.TVCActuatorAngles[i] = Physics::lerp(a.TVCActuatorAngles[i], b.TVCActuatorAngles[i], x);
	}

	void DSS::saveGasThrusterState(const Physics::Hardware::GasThruster& source, DSS::GasThrusterState& dest) {
		dest.thrustMagnitude = source.mThrustMagnitude;
		dest.thrust_stage = source.mThrust_stage;
		dest.active = source.mActive;
		dest.currentMassFlowRate = source.mCurrentMassFlowRate;
	}

	void DSS::loadGasThrusterState(const DSS::GasThrusterState& source, Physics::Hardware::GasThruster& dest) {
		dest.mThrustMagnitude = source.thrustMagnitude;
		dest.mThrust_stage = source.thrust_stage;
		dest.mActive = source.active;
		dest.mCurrentMassFlowRate = source.currentMassFlowRate;
	}

	void DSS::lerpGasThrusterState(const DSS::GasThrusterState& a, const DSS::GasThrusterState& b, double x, DSS::GasThrusterState& dest) {
		dest.thrustMagnitude = Physics::lerp(a.thrustMagnitude, b.thrustMagnitude, x);
		dest.thrust_stage = glm::lerp(a.thrust_stage, b.thrust_stage, x);
		dest.active = a.active;
		dest.currentMassFlowRate = Physics::lerp(a.currentMassFlowRate, b.currentMassFlowRate, x);
	}

	void DSS::saveLandingLegState(const Physics::Hardware::LandingLeg& source, DSS::Falcon9::Stage1::LandingLegState& dest) {
		dest.deploymentPhase = static_cast<DSS::Falcon9::Stage1::LandingLegState::Phase>(source.mDeploymentPhase);
		dest.deploymentAngle_rads = source.mDeploymentAngle_rads;
		dest.deploymentVelocity_rads = source.mDeploymentVelocity_rads;
		dest.alongPiston_stage3D = source.mAlongTPiston_stage3D;
		dest.pistonEndPos_stage3D = source.mPistonEndPos_stage3D;
		dest.alongPusher_stage3D = source.mAlongPusher_stage3D;
		dest.pusherEndPos_stage3D = source.mPusherEndPos_stage3D;
		dest.compToStage = source.mCompToStage;

		saveLegPistonState(*source.getPiston(), dest.telescopingPiston);
		saveLegPusherState(*source.mPusher.get(), dest.pusher);
	}

	void DSS::loadLandingLegState(const DSS::Falcon9::Stage1::LandingLegState& source, Physics::Hardware::LandingLeg& dest) {
		dest.mDeploymentPhase = static_cast<Hardware::LandingLeg::Phase>(source.deploymentPhase);
		dest.mDeploymentAngle_rads = source.deploymentAngle_rads;
		dest.mDeploymentVelocity_rads = source.deploymentVelocity_rads;
		dest.mAlongTPiston_stage3D = source.alongPiston_stage3D;
		dest.mPistonEndPos_stage3D = source.pistonEndPos_stage3D;
		dest.mAlongPusher_stage3D = source.alongPusher_stage3D;
		dest.mPusherEndPos_stage3D = source.pusherEndPos_stage3D;
		dest.mCompToStage = source.compToStage;

		loadLegPistonState(source.telescopingPiston, *dest.mPiston.get());
		loadLegPusherState(source.pusher, *dest.mPusher.get());
	}

	void DSS::lerpLandingLegState(const DSS::Falcon9::Stage1::LandingLegState& a, const DSS::Falcon9::Stage1::LandingLegState& b, double x, DSS::Falcon9::Stage1::LandingLegState& dest) {
		dest.deploymentPhase = a.deploymentPhase;
		dest.deploymentAngle_rads = Physics::lerp(a.deploymentAngle_rads, b.deploymentAngle_rads, x);
		dest.deploymentVelocity_rads = Physics::lerp(a.deploymentVelocity_rads, b.deploymentVelocity_rads, x);
		dest.alongPiston_stage3D = glm::lerp(a.alongPiston_stage3D, b.alongPiston_stage3D, x);
		dest.pistonEndPos_stage3D = glm::lerp(a.pistonEndPos_stage3D, b.pistonEndPos_stage3D, x);
		dest.alongPusher_stage3D = glm::lerp(a.alongPusher_stage3D, b.alongPusher_stage3D, x);
		dest.pusherEndPos_stage3D = glm::lerp(a.pusherEndPos_stage3D, b.pusherEndPos_stage3D, x);
		dest.compToStage = ::lerp(a.compToStage, b.compToStage, x);

		lerpLegPistonState(a.telescopingPiston, b.telescopingPiston, x, dest.telescopingPiston);
		lerpLegPusherState(a.pusher, b.pusher, x, dest.pusher);
	}

	void DSS::saveGridFinState(const Physics::Hardware::GridFin& source, DSS::Falcon9::Stage1::GridFinState& dest) {
		dest.currentPhase = static_cast<DSS::Falcon9::Stage1::GridFinState::Phase>(source.mCurrentPhase);
		dest.rollAngle = source.mRollAngle;
		dest.pitchAngle = source.mPitchAngle;
		dest.percentDeployed = source.mPercentDeployed;
		dest.compToStage = source.mCompToStage;
	}

	void DSS::loadGridFinState(const DSS::Falcon9::Stage1::GridFinState& source, Physics::Hardware::GridFin& dest) {
		dest.mCurrentPhase = static_cast<Hardware::GridFin::Phase>(source.currentPhase);
		dest.mRollAngle = source.rollAngle;
		dest.mPitchAngle = source.pitchAngle;
		dest.mPercentDeployed = source.percentDeployed;
		dest.mCompToStage = source.compToStage;
	}

	void DSS::lerpGridFinState(const DSS::Falcon9::Stage1::GridFinState& a, const DSS::Falcon9::Stage1::GridFinState& b, double x, DSS::Falcon9::Stage1::GridFinState& dest) {
		dest.currentPhase = a.currentPhase;
		dest.rollAngle = Physics::lerp(a.rollAngle, b.rollAngle, x);
		dest.pitchAngle = Physics::lerp(a.pitchAngle, b.pitchAngle, x);
		dest.percentDeployed = Physics::lerp(a.percentDeployed, b.percentDeployed, x);
		dest.compToStage = ::lerp(a.compToStage, b.compToStage, x);
	}

	void DSS::saveLegPistonState(const Physics::Hardware::TelescopingPiston& source, DSS::Falcon9::Stage1::LandingLegState::TelescopingPistonState& dest) {
		dest.currentLength = source.mCurrentLength;
		dest.lastLength = source.mLastLength;
		dest.extensionRate = source.mExtensionRate;
	}

	void DSS::loadLegPistonState(const DSS::Falcon9::Stage1::LandingLegState::TelescopingPistonState& source, Physics::Hardware::TelescopingPiston& dest) {
		dest.mCurrentLength = source.currentLength;
		dest.mLastLength = source.lastLength;
		dest.mExtensionRate = source.extensionRate;
	}

	void DSS::lerpLegPistonState(
		const DSS::Falcon9::Stage1::LandingLegState::TelescopingPistonState& a,
		const DSS::Falcon9::Stage1::LandingLegState::TelescopingPistonState& b,
		double x, 
		DSS::Falcon9::Stage1::LandingLegState::TelescopingPistonState& dest) 
	{
		dest.currentLength = Physics::lerp(a.currentLength, b.currentLength, x);
		dest.lastLength = Physics::lerp(a.lastLength, b.lastLength, x);
		dest.extensionRate = Physics::lerp(a.extensionRate, b.extensionRate, x);
	}

	void DSS::saveLegPusherState(const Physics::Hardware::LegDeploymentActuator& source, DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& dest) {
		dest.springLength = source.mSpring.getCurrentLength();
		dest.springCompressionRate = source.mSpring.getCompressionRate();
	}

	void DSS::loadLegPusherState(const DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& source, Physics::Hardware::LegDeploymentActuator& dest) {
		dest.mSpring.update(source.springLength, source.springCompressionRate);
	}

	void DSS::lerpLegPusherState(
		const DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& a, 
		const DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& b, 
		double x,
		DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& dest)
	{
		dest.springLength = Physics::lerp(a.springLength, b.springLength, x);
		dest.springCompressionRate = Physics::lerp(a.springCompressionRate, b.springCompressionRate, x);
	}

}