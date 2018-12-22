#include "Falcon9Stage2.h"
#include "Physics/External/Environment.h"

namespace Physics {
	namespace Hardware {

		Falcon9Stage2::Falcon9Stage2() {
			setStageSpecificParams();
			addEngine();
			addThrusters();
			addTanks();
			addFluidLines();
			preFlight_temp();

			mergeTotalMass_stage();
			mergeTotalInertia_stage();
		}

		void Falcon9Stage2::preFlight_temp() {
			//Load correct amounts of propellant into the tanks
			mPropellantSupplies.addToTank(Propellants::liquidOxygen, 75.2_tonnes);
			mPropellantSupplies.addToTank(Propellants::RP1, 32.3_tonnes);
			//static_cast<PropellantTank*>(mThrusterGasSupply[0])->addFluid(8.0);
		}

		void Falcon9Stage2::stageSpecificUpdates(double t, double dt) { }

		std::vector<Force_world> Falcon9Stage2::stageSpecificForces_world() const {
			//Not returning any other forces at the moment.
			std::vector<Force_world> stageSpecificForces_world;

			//push_back forces in here
			//stageSpecificForces_world.push_back(Force_world(someForce, someApplicationPointOnF9S1))

			return stageSpecificForces_world;
		}

		glm::dvec3 Falcon9Stage2::stageSpecificTorques_world() const {
			return glm::dvec3(0.0);
		}

		void Falcon9Stage2::addEngine() {
			//TODO: Work out the correct height of the engine on the stage
			mEngines.addComponent(std::make_unique<Merlin1DVac>(glm::dvec3(0.0, -1.3, 0.0)));
		}

		void Falcon9Stage2::addThrusters() {
			//TODO: Add the gas thrusters to the second stage
		}

		void Falcon9Stage2::addTanks() {
			mPropellantSupplies.addComponent(std::make_unique<FluidTank>(
				glm::dvec3(0.0, 4.3, 0.0), 
				Propellants::mFluids[Propellants::liquidOxygen],
				5.8 * 1.05633727, 
				mDiameter,
				0.012, 
				2'550.0
			));
			
			mPropellantSupplies.addComponent(std::make_unique<FluidTank>(
				glm::dvec3(0.0, 0.6, 0.0), 
				Propellants::mFluids[Propellants::RP1],
				3.69536608,
				mDiameter, 
				0.012, 
				2'550.0
			));
			
			//mFuelSupplies.addComponent(std::make_unique<PropellantTank>(glm::dvec3(0.0, 40.0, 0.0), 0.36, 0.36, 2550.0, 0.0042, 280.0, Propellant::State::gas)); //N2
		}

		void Falcon9Stage2::addFluidLines() {
			//Connects the propellant tanks to the engine
			{
				FluidTank& oxidiserTank = *mPropellantSupplies.get<FluidTank>(Propellants::liquidOxygen);
				FluidTank& fuelTank = *mPropellantSupplies.get<FluidTank>(Propellants::RP1);
				PropSupplyLine& engineSupplyLine = mPropellantSupplies.addPropSupplyLine(oxidiserTank, fuelTank);

				mEngines.get<Merlin1DVac>(0)->attachPropSupplyLine(&engineSupplyLine);
			}
		}

		void Falcon9Stage2::setStageSpecificParams() {
			mDiameter = 3.66;
			mHeight = 8.73197;

			//mPosition_vehicle = glm::dvec3(0.0, 46.02762, 0.0);
			//TODO - Make sure mMiscInertMass of stage is accurate and accounts for all other mass that isn't already part of sub components like Engines or Tanks etc
			mMiscInertMass = 3.9_tonnes;
		}
		
		Mass Falcon9Stage2::stageSpecificMass_stage() const {
			return 0.0;
		}

		InertiaTensor Falcon9Stage2::stageSpecificCoMInertia_stage() const {
			return InertiaTensor();
		}

	}
}