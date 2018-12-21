#include "Falcon9Stage1.h"
#include "Physics/External/Environment.h"

namespace Physics {
	namespace Hardware {

		Falcon9Stage1::Falcon9Stage1() {
			setStageSpecificParams();
			addEngines();
			addThrusters();
			addTanks();
			addFluidLines();
			preFlight_temp();

			mergeTotalMass_stage();
			mergeTotalInertia_stage();	
		}

		void Falcon9Stage1::preFlight_temp() 
			//temp - All operations will be handled autonomously eventually.
		{
			//Load correct amounts of propellant into the tanks
#if LOAD_ENGINE_PROPELLANT
			mPropellantSupplies.addToTank(Propellants::liquidOxygen, 287.430_tonnes);
			mPropellantSupplies.addToTank(Propellants::RP1, 123.570_tonnes);
#endif
			mPropellantSupplies.addToTank(Propellants::nitrogen, 8.0);
		}

		void Falcon9Stage1::stageSpecificUpdates(double t, double dt) {
			mGridFins.update(dt/* , External::Environment::getAirDensity_kg_per_m3(static_cast<int>(floor(mState.getCMPosition_world().y))), mFlowVelocity_local */);
			mLandingLegs.update(*this, dt);

			//Rotate all engines
			for(unsigned char i = 0; i < mEngines.getCount(); i++) {
				//mEngines.getComponent<Merlin1D>(i)->gimbalTo(0.0, (sin(t) + 1.0) / 2.0 * 6.0);
				mEngines.getComponent<Merlin1D>(i)->setActive(true);
			}
		}

		std::vector<Force_world> Falcon9Stage1::stageSpecificForces_world() const 
			//TODO: Add custom forces generated by the first stage, like drag from grid fins for example
		{
			//Not returning any other forces at the moment.
			std::vector<Force_world> otherForces_world;

			//push_back forces in here
			//otherForces_world.push_back(Force_world(someForce, someApplicationPointOnF9S1))

			return otherForces_world;
		}

		glm::dvec3 Falcon9Stage1::stageSpecificTorques_world() const {
			return mState.getOrientation_world() * mGridFins.getTotalTorque_stage();
		}

		void Falcon9Stage1::addEngines() {
			using namespace glm;

			//The height of the gimbal (the engines origin) above the dance floor in metres
			const double aboveDanceFloor = 0.70534;

			//Central engine								
			mEngines.addComponent(std::make_unique<Merlin1D>(CoordTransform3D(dvec3(0.0, aboveDanceFloor - 0.13, 0.0), rotate(radians(-143.12), dvec3(0.0, 1.0, 0.0)))));

			//8 outboard engines
			for (unsigned i = 0; i < 8; i++) {
				const double angle = -15.6095 + i * 45.0;
				const dvec3 pointer = rotate(dvec3(0.0, aboveDanceFloor, 1.33853), radians(angle), { 0.0, 1.0, 0.0 });

				mEngines.addComponent(std::make_unique<Merlin1D>(CoordTransform3D(pointer, rotate(radians(angle + 90.0), dvec3(0.0, 1.0, 0.0)))));
			}
		}

		void Falcon9Stage1::addThrusters() {
			using namespace glm;

			const double clustersHeight = 40.225;

			const dvec3
				leftGroupPos_stage = dvec3(0.0, clustersHeight, 1.88),
				rightGroupPos_stage = dvec3(0.0, clustersHeight, -1.88);

			//Left cluster
			mThrusters.addComponent(std::make_unique<GasThruster>(CoordTransform3D(leftGroupPos_stage + dvec3(0.0, 0.05, 0.0), rotate(radians(90.0), dvec3(-1.0, 0.0, 0.0))))); //outward
			mThrusters.addComponent(std::make_unique<GasThruster>(CoordTransform3D(leftGroupPos_stage, rotate(radians(90.0), dvec3(0.0, 0.0, -1.0))))); 				        //left
			mThrusters.addComponent(std::make_unique<GasThruster>(CoordTransform3D(leftGroupPos_stage, rotate(radians(90.0), dvec3(0.0, 0.0, 1.0)))));  				        //right
			mThrusters.addComponent(std::make_unique<GasThruster>(CoordTransform3D(leftGroupPos_stage)));														                //ullage

			//Right cluster	 
			mThrusters.addComponent(std::make_unique<GasThruster>(CoordTransform3D(rightGroupPos_stage + dvec3(0.0, 0.05, 0.0), rotate(radians(90.0), dvec3(1.0, 0.0, 0.0)))));  //outward
			mThrusters.addComponent(std::make_unique<GasThruster>(CoordTransform3D(rightGroupPos_stage, rotate(radians(90.0), dvec3(0.0, 0.0, -1.0)))));                         //left
			mThrusters.addComponent(std::make_unique<GasThruster>(CoordTransform3D(rightGroupPos_stage, rotate(radians(90.0), dvec3(0.0, 0.0, 1.0)))));                          //right
			mThrusters.addComponent(std::make_unique<GasThruster>(CoordTransform3D(rightGroupPos_stage)));                                                                       //ullage
		}

		void Falcon9Stage1::addTanks() {
			const double
				tankWallDensity = 2'550.0, //2'550.0
				mainPropTankWallThickness = 0.012;

			mPropellantSupplies.addComponent(std::make_unique<FluidTank>(
				glm::dvec3(0.0, 16.38, 0.0), 
				Propellants::mFluids[Propellants::liquidOxygen],
				22.8, 
				mDiameter, 
				mainPropTankWallThickness, 
				tankWallDensity
				));
			
			mPropellantSupplies.addComponent(std::make_unique<FluidTank>(
				glm::dvec3(0.0, 2.3, 0.0), 
				Propellants::mFluids[Propellants::RP1],
				14.0, 
				mDiameter,
				mainPropTankWallThickness,
				tankWallDensity 
				));
			
			mPropellantSupplies.addComponent(std::make_unique<FluidTank>(
				glm::dvec3(0.0, 40.0, 0.0),
				Propellants::mFluids[Propellants::nitrogen],
				0.36, 
				0.36, 
				0.042, 
				tankWallDensity
				));
		}

		void Falcon9Stage1::addFluidLines() {
			//Connects the propellant tanks to the engines
			{
				FluidTank& oxidiserTank = *mPropellantSupplies.getComponent<FluidTank>(Propellants::liquidOxygen);
				FluidTank& fuelTank = *mPropellantSupplies.getComponent<FluidTank>(Propellants::RP1);
				PropSupplyLine& engineSupplyLine = mPropellantSupplies.addPropSupplyLine(oxidiserTank, fuelTank);

				for (const auto& e : mEngines.getAllComponents())
					static_cast<Engine*>(e.get())->attachPropSupplyLine(&engineSupplyLine);
			}

			//Connects the nitrogen gas tank to the thrusters
			{
				FluidTank& nitrogenTank = *mPropellantSupplies.getComponent<FluidTank>(Propellants::nitrogen);
				GasSupplyLine& gasThrusterSupplyLine = mPropellantSupplies.addGasSupplyLine(nitrogenTank);

				for (const auto& g : mThrusters.getAllComponents())
					static_cast<GasThruster*>(g.get())->attachPropSupplyLine(&gasThrusterSupplyLine);
			}
		}

		void Falcon9Stage1::setStageSpecificParams() {
			mHeight = 47.0;
			mDiameter = 3.66;
			mMiscInertMass = 9.420438_tonnes; //9.420438_tonnes
		}

		Mass Falcon9Stage1::stageSpecificMass_stage() const {
			return
				mGridFins.getTotalMass_stage() +
				mLandingLegs.getTotalMass_stage();
		}

		InertiaTensor Falcon9Stage1::stageSpecificCoMInertia_stage() const {
			using namespace glm;

			InertiaTensor output;
			dvec3
				displacement,
				stageCentreMass_stage = mState.getMass_local().getCentre();
			double mass = 0.0;

			//Adding the moment of inertia of the grid fins
			displacement = mGridFins.getTotalMass_stage().getCentre() - stageCentreMass_stage;
			mass = mGridFins.getTotalMass_stage().getValue();
			output += InertiaTensor::parallelAxis(mGridFins.getTotalCoMInertia_stage(), mass, displacement);

			//Adding the moment of inertia of the landing legs
			displacement = mLandingLegs.getTotalMass_stage().getCentre() - stageCentreMass_stage;
			mass = mLandingLegs.getTotalMass_stage().getValue();
			output += InertiaTensor::parallelAxis(mLandingLegs.getTotalCoMInertia_stage(), mass, displacement);

			//test
			//distance = mSingleComponent.getTotalMass(StageComponent::CoordSpace::STAGE).getCentre() - mActivePivotPoint_stage;
			//mass = mSingleComponent.getTotalMass(StageComponent::CoordSpace::STAGE).getValue();
			//output += InertiaTensor::parallelAxis(mSingleComponent.get)

			return output;
		}

	}
} 