#include "Falcon9Stage1.h"
#include "Physics/External/Environment.h"

namespace Physics {
	namespace Hardware {

		Falcon9Stage1::Falcon9Stage1() {
			setStageSpecificParams();

			addEngines();
			addThrusters();
			addTanks();
			addPropellantLines();

			preFlight_temp();

			initState();
		}

		void Falcon9Stage1::checkInput_temp(double dt) {
			using namespace GF;

			//Engines
			{
				if (Input::isKeyPressed(GLFW_KEY_L)) {
					for (const auto& e : mEngines.getAllComponents())
						static_cast<Engine*>(e.get())->setActive(true);
				}
				else if (Input::isKeyPressed(GLFW_KEY_3)) {
					static_cast<Engine*>(mEngines.getAllComponents()[0].get())->setActive(true);
					static_cast<Engine*>(mEngines.getAllComponents()[1].get())->setActive(true);
					static_cast<Engine*>(mEngines.getAllComponents()[5].get())->setActive(true);
				}
				else if (Input::isKeyPressed(GLFW_KEY_1)) {
					static_cast<Engine*>(mEngines.getAllComponents()[0].get())->setActive(true);
				}

				if (Input::isKeyPressed(GLFW_KEY_P)) {
					for (const auto& e : mEngines.getAllComponents())
						static_cast<Engine*>(e.get())->setActive(false);
				}

				//Numpad
				if (Input::isKeyPressed(GLFW_KEY_KP_5))
					static_cast<Engine*>(mEngines.getAllComponents()[0].get())->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_KP_4))
					static_cast<Engine*>(mEngines.getAllComponents()[1].get())->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_KP_6))
					static_cast<Engine*>(mEngines.getAllComponents()[5].get())->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_KP_2))
					static_cast<Engine*>(mEngines.getAllComponents()[3].get())->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_KP_8))
					static_cast<Engine*>(mEngines.getAllComponents()[7].get())->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_KP_1))
					static_cast<Engine*>(mEngines.getAllComponents()[2].get())->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_KP_7))
					static_cast<Engine*>(mEngines.getAllComponents()[4].get())->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_KP_9))
					static_cast<Engine*>(mEngines.getAllComponents()[6].get())->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_KP_3))
					static_cast<Engine*>(mEngines.getAllComponents()[8].get())->setActive(true);
			}

			//Gas thrusters
			{
				IThrustGenerator* temp = nullptr;
				for (unsigned char i = 0; i < mThrusters.getAllComponents().size(); i++) {
					temp = static_cast<GasThruster*>(mThrusters[i]);
					temp->setActive(false);
				}

#if THRUSTER_SPIN
				//Induces spin
				if (Input::isKeyPressed(GLFW_KEY_UP)) {
					static_cast<GasThruster*>(mThrusters[1])->setActive(true);
					static_cast<GasThruster*>(mThrusters[6])->setActive(true);
				}

				if (Input::isKeyPressed(GLFW_KEY_DOWN)) {
					static_cast<GasThruster*>(mThrusters[2])->setActive(true);
					static_cast<GasThruster*>(mThrusters[5])->setActive(true);
				}
#else
				if (Input::isKeyPressed(GLFW_KEY_UP)) static_cast<GasThruster*>(mThrusters[0])->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_DOWN)) static_cast<GasThruster*>(mThrusters[3])->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_LEFT)) static_cast<GasThruster*>(mThrusters[1])->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_RIGHT)) static_cast<GasThruster*>(mThrusters[2])->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_U)) static_cast<GasThruster*>(mThrusters[4])->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_K)) static_cast<GasThruster*>(mThrusters[5])->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_H)) static_cast<GasThruster*>(mThrusters[6])->setActive(true);
				if (Input::isKeyPressed(GLFW_KEY_J)) static_cast<GasThruster*>(mThrusters[7])->setActive(true);
#endif					
			}

			if (Input::isKeyReleased(GLFW_KEY_HOME))
				mState.setAngularVelocity_world(glm::dvec3(0.0));

			//Grid_fins
			{
				double rotationSpeed = 200.0;
				static double tempForInput = 0.0;

				if (Input::isKeyPressed(GLFW_KEY_Q)) {
					for (const auto& g : mGridFins.getAllComponents())
						static_cast<GridFin*>(g.get())->deploy();
				}
				if (Input::isKeyPressed(GLFW_KEY_V)) {
					for (const auto& l : mLandingLegs.getAllComponents())
						static_cast<LandingLeg*>(l.get())->deploy();
				}

				if (Input::isKeyPressed(GLFW_KEY_LEFT)) {
					tempForInput += rotationSpeed * dt;
					if (tempForInput > 45.0) tempForInput = 45.0;

					for (const auto& g : mGridFins.getAllComponents())
						static_cast<GridFin*>(g.get())->setRoll(tempForInput);
				}
				if (Input::isKeyPressed(GLFW_KEY_RIGHT)) {
					tempForInput -= rotationSpeed * dt;
					if (tempForInput < -45.0) tempForInput = -45.0;

					for (const auto& g : mGridFins.getAllComponents())
						static_cast<GridFin*>(g.get())->setRoll(tempForInput);
				}
			}

			if (Input::isKeyPressed(GLFW_KEY_G)) {
				//mState.reset();
				mState.setPosition_world({ 0.0, 0.0, 0.0 });
			}

			if (Input::isKeyPressed(GLFW_KEY_R)) {
				mState.reset();
				mState.setPosition_world({ 0.0, 2'800.0, -900.0 });
				mState.setVelocity_world({ 0.0, 0.0, 40.0 });
				mState.setOrientation_world(rotate(mState.getOrientation_world(), glm::radians(-80.0), { 1.0, 0.0, 0.0 }));
				
				static_cast<FluidTank*>(mPropellantSupplies.getComponent(Propellants::liquidOxygen))->removeAllPropellant();
				static_cast<FluidTank*>(mPropellantSupplies.getComponent(Propellants::liquidOxygen))->addPropellant(287.430_tonnes * 0.12);

				static_cast<FluidTank*>(mPropellantSupplies.getComponent(Propellants::RP1))->removeAllPropellant();
				static_cast<FluidTank*>(mPropellantSupplies.getComponent(Propellants::RP1))->addPropellant(123.570_tonnes * 0.12);
			}
		}

		void Falcon9Stage1::preFlight_temp() 
			//temp - All operations will be handled autonomously eventually.
		{
			//Load correct amounts of propellant into the tanks
#if LOAD_ENGINE_PROPELLANT
			static_cast<FluidTank*>(mPropellantSupplies[Propellants::liquidOxygen])->addPropellant(287.430_tonnes); //287430.0
			static_cast<FluidTank*>(mPropellantSupplies[Propellants::RP1])->addPropellant(123.570_tonnes); //123570.0
#endif
			static_cast<FluidTank*>(mPropellantSupplies[Propellants::nitrogen])->addPropellant(8.0);

			//mPropellantSupplies.update();
		}

		void Falcon9Stage1::otherUpdates(double t, double dt) {
			mGridFins.update(dt/* , External::Environment::getAirDensity_kg_per_m3(static_cast<int>(floor(mState.getCMPosition_world().y))), mFlowVelocity_local */);
			mLandingLegs.update(*this, dt);
		}

		std::vector<Force_world> Falcon9Stage1::otherForces_world() const 
			//TODO: Add custom forces generated by the first stage, like drag from grid fins for example
		{
			//Not returning any other forces at the moment.
			std::vector<Force_world> otherForces_world;

			//push_back forces in here
			//otherForces_world.push_back(Force_world(someForce, someApplicationPointOnF9S1))

			return otherForces_world;
		}

		glm::dvec3 Falcon9Stage1::otherTorques_world() const {
			//The only other torque specific to this stage AT THE MOMENT is that which is generated by the grid fins
			return mState.getOrientation_world() * mGridFins.getTotalTorque_stage();
		}

		void Falcon9Stage1::addEngines() {
			using namespace glm;

			const double heightAboveBase = 0.70534;

			//Central engine								
			mEngines.addComponent(std::make_unique<Merlin1D>(GF::CoordTransform3D(dvec3(0.0, heightAboveBase - 0.13, 0.0), rotate(dmat4(1.0), radians(-143.12), { 0.0, 1.0, 0.0 }))));

			for (int i = 0; i < 8; i++) {
				double angle = -15.6095 + i * 45.0;
				dvec3 pointer = rotate(dvec3(0.0, heightAboveBase, 1.33853), radians(angle), { 0.0, 1.0, 0.0 });

				mEngines.addComponent(std::make_unique<Merlin1D>(GF::CoordTransform3D(pointer, rotate(dmat4(1.0), radians(angle + 90.0), { 0.0, 1.0, 0.0 }))));
			}
		}

		void Falcon9Stage1::addThrusters() {
			using namespace glm;

			double clustersHeight = 40.3;

			dvec3
				leftGroupPos = dvec3(0.0, clustersHeight, 1.88),
				rightGroupPos = dvec3(0.0, clustersHeight, -1.88);

			//Left cluster
			mThrusters.addComponent(std::make_unique<GasThruster>(GF::CoordTransform3D(leftGroupPos + dvec3(0.0, 0.05, 0.0), toQuat(rotate(radians(90.0), dvec3(-1.0, 0.0, 0.0)))))); //outward
			mThrusters.addComponent(std::make_unique<GasThruster>(GF::CoordTransform3D(leftGroupPos, toQuat(rotate(radians(90.0), dvec3(0.0, 0.0, -1.0)))))); //left
			mThrusters.addComponent(std::make_unique<GasThruster>(GF::CoordTransform3D(leftGroupPos, toQuat(rotate(radians(90.0), dvec3(0.0, 0.0, 1.0))))));  //right
			mThrusters.addComponent(std::make_unique<GasThruster>(GF::CoordTransform3D(leftGroupPos)));														  //down

			//Right cluster	 
			mThrusters.addComponent(std::make_unique<GasThruster>(GF::CoordTransform3D(rightGroupPos + dvec3(0.0, 0.05, 0.0), toQuat(rotate(radians(90.0), dvec3(1.0, 0.0, 0.0))))));  //outward
			mThrusters.addComponent(std::make_unique<GasThruster>(GF::CoordTransform3D(rightGroupPos, toQuat(rotate(radians(90.0), dvec3(0.0, 0.0, -1.0)))))); //left
			mThrusters.addComponent(std::make_unique<GasThruster>(GF::CoordTransform3D(rightGroupPos, toQuat(rotate(radians(90.0), dvec3(0.0, 0.0, 1.0))))));  //right
			mThrusters.addComponent(std::make_unique<GasThruster>(GF::CoordTransform3D(rightGroupPos)));                                                       //down
		}

		void Falcon9Stage1::addTanks() {
			const double
				tankWallDensity = 2'550.0,
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

		void Falcon9Stage1::addPropellantLines() {
			//Connects the propellant tanks to the engines
			{
				FluidTank& oxidiserTank = *static_cast<FluidTank*>(mPropellantSupplies[Propellants::liquidOxygen]);
				FluidTank& fuelTank = *static_cast<FluidTank*>(mPropellantSupplies[Propellants::RP1]);
				PropSupplyLine& engineSupplyLine = mPropellantSupplies.addPropSupplyLine(oxidiserTank, fuelTank);

				for (const auto& e : mEngines.getAllComponents())
					static_cast<Engine*>(e.get())->attachPropSupplyLine(&engineSupplyLine);
			}

			//Connects the nitrogen gas tank to the thrusters
			{
				FluidTank& nitrogenTank = *static_cast<FluidTank*>(mPropellantSupplies[Propellants::nitrogen]);
				GasSupplyLine& gasThrusterSupplyLine = mPropellantSupplies.addGasSupplyLine(nitrogenTank);

				for (const auto& g : mThrusters.getAllComponents())
					static_cast<GasThruster*>(g.get())->attachPropSupplyLine(&gasThrusterSupplyLine);
			}
		}

		void Falcon9Stage1::setStageSpecificParams() {
			mHeight = 47.0;
			mDiameter = 3.66;
			mMiscInertMass = 9.420438_tonnes;
		}

		Mass Falcon9Stage1::otherMass_stage() const {
			return
				mGridFins.getTotalMass_stage() +
				mLandingLegs.getTotalMass_stage();
		}

		InertiaTensor Falcon9Stage1::otherCmInertia_stage() const {
			using namespace glm;
			

			InertiaTensor output;
			dvec3
				displacement,
				stageCentreMass_stage = mState.getMass_local().getCentre();
			double mass = 0.0;

			//Adding the moment of inertia of the grid fins
			displacement = mGridFins.getTotalMass_stage().getCentre() - stageCentreMass_stage;
			mass = mGridFins.getTotalMass_stage().getValue();
			output += InertiaTensor::parallelAxis(mGridFins.getTotalCmInertia_stage(), mass, displacement);

			//Adding the moment of inertia of the landing legs
			displacement = mLandingLegs.getTotalMass_stage().getCentre() - stageCentreMass_stage;
			mass = mLandingLegs.getTotalMass_stage().getValue();
			output += InertiaTensor::parallelAxis(mLandingLegs.getTotalCmInertia_stage(), mass, displacement);

			//test
			//distance = mSingleComponent.getTotalMass(StageComponent::CoordSpace::STAGE).getCentre() - mActivePivotPoint_stage;
			//mass = mSingleComponent.getTotalMass(StageComponent::CoordSpace::STAGE).getValue();
			//output += InertiaTensor::parallelAxis(mSingleComponent.get)

			return output;
		}

	}
} 