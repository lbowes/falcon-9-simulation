#include "VehicleOverlay.h"
#include "Physics/Hardware/Falcon_9/Falcon9.h"
								  
namespace Graphics {

	VehicleOverlay::VehicleOverlay(const Physics::Hardware::Falcon9& simDataSource, float windowAspect) :
		mDataSource(simDataSource),
		mMarker2DOverlay(mResourceContainer, mRenderer)
	{
		load(windowAspect);
	}

	void VehicleOverlay::render(glm::mat4 viewProjection, float windowAspect, glm::vec2 windowDimensions) {
		mOrthoCam->setAspect(windowAspect);
		
		updateAllMarkers();
		//mMarker2DOverlay.render(viewProjection, windowAspect, windowDimensions);
	}

	void VehicleOverlay::load(float windowAspect) {
		addAllMarkers();

		mOrthoCam = std::make_unique<GF::OrthographicCamera>(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, -1.0f, windowAspect, 1.0f);
		mRenderer.setCamera(*mOrthoCam);
	}

	void VehicleOverlay::addAllMarkers() {
		//Launch vehicle
		{
			mMarker2DOverlay.addMarker(mMarkedLocations.origin, Marker2D::Type::origin, "F9 launch vehicle");
			mMarker2DOverlay.addMarker(mMarkedLocations.centreMass, Marker2D::Type::centreMass, "F9 launch vehicle");
		}

		//Stage_1
		{
			mMarker2DOverlay.addMarker(mMarkedLocations.S1Origin, Marker2D::Type::origin, "F9 Stage_1");
			mMarker2DOverlay.addMarker(mMarkedLocations.S1CentreMass, Marker2D::Type::centreMass, "F9 Stage_1");

			//Engines
			for (unsigned char i = 0; i < 9; i++) {
				std::string id = std::to_string(i);
				mMarker2DOverlay.addMarker(mMarkedLocations.enginesOrigins[i], Marker2D::Type::origin, "F9 merlin 1D No." + id);
				mMarker2DOverlay.addMarker(mMarkedLocations.enginesCoM[i], Marker2D::Type::centreMass, "F9 merlin 1D No." + id);
			}

			//Landing_legs and grid fins
			for (unsigned char i = 0; i < 4; i++) {
				std::string id = std::to_string(i);
				mMarker2DOverlay.addMarker(mMarkedLocations.legsOrigins[i], Marker2D::Type::origin, "F9 landing leg No." + id);
				mMarker2DOverlay.addMarker(mMarkedLocations.legsCoM[i], Marker2D::Type::centreMass, "F9 landing leg No." + id);
				mMarker2DOverlay.addMarker(mMarkedLocations.gridFinsOrigins[i], Marker2D::Type::origin, "F9 grid fin No." + id);
				mMarker2DOverlay.addMarker(mMarkedLocations.gridFinsCoM[i], Marker2D::Type::centreMass, "F9 grid fin No." + id);
			}

			//LOX tank
			mMarker2DOverlay.addMarker(mMarkedLocations.tanksOrigins[Physics::Propellants::liquidOxygen], Marker2D::Type::origin, "F9 LOX tank");
			mMarker2DOverlay.addMarker(mMarkedLocations.tanksCoM[Physics::Propellants::liquidOxygen], Marker2D::Type::centreMass, "F9 LOX tank");

			//RP-1 tank
			mMarker2DOverlay.addMarker(mMarkedLocations.tanksOrigins[Physics::Propellants::RP1], Marker2D::Type::origin, "F9 RP-1 tank");
			mMarker2DOverlay.addMarker(mMarkedLocations.tanksCoM[Physics::Propellants::RP1], Marker2D::Type::centreMass, "F9 RP-1 tank");
		}

		//Stage_2
		{
			mMarker2DOverlay.addMarker(mMarkedLocations.S2Origin, Marker2D::Type::origin);
			mMarker2DOverlay.addMarker(mMarkedLocations.S2CentreMass, Marker2D::Type::centreMass);
		}
	}

	void VehicleOverlay::updateAllMarkers() {
		//Launch vehicle
		{
			const State &vehicleState = mDataSource.getState();
			mMarkedLocations.origin = vehicleState.getCoMPosition_world();
			mMarkedLocations.centreMass = vehicleState.getObjectSpace().toParentSpace(vehicleState.getMass_local().getCentre());
		}

		//Stage_1
		{
			using namespace Physics::Hardware;

			const State &stage1State = mDataSource.getStage1().getState();
			mMarkedLocations.S1CentreMass = stage1State.getObjectSpace().toParentSpace(stage1State.getMass_local().getCentre());
			mMarkedLocations.S1Origin = stage1State.getObjectSpace().toParentSpace();

			//Engines
			for (unsigned char i = 0; i < 9; i++) {
				IStageComponent* comp = mDataSource.getStage1().getEngines().getComponent(i);
				mMarkedLocations.enginesCoM[i] = stage1State.getObjectSpace().toParentSpace(comp->getMass(IStageComponent::CoordSpace::stage).getCentre());
				mMarkedLocations.enginesOrigins[i] = stage1State.getObjectSpace().toParentSpace(comp->getCompToStageTransform().toParentSpace());
			}

			for (unsigned char i = 0; i < 4; i++) {
				//Landing_legs
				IStageComponent* comp = mDataSource.getStage1().getLandingLegs().getComponent(i);
				mMarkedLocations.legsCoM[i] = stage1State.getObjectSpace().toParentSpace(comp->getMass(IStageComponent::CoordSpace::stage).getCentre());
				mMarkedLocations.legsOrigins[i] = stage1State.getObjectSpace().toParentSpace(comp->getCompToStageTransform().toParentSpace());

				//Grid_fins
				comp = mDataSource.getStage1().getGridFins().getComponent(i);
				mMarkedLocations.gridFinsCoM[i] = stage1State.getObjectSpace().toParentSpace(comp->getMass(IStageComponent::CoordSpace::stage).getCentre());
				mMarkedLocations.gridFinsOrigins[i] = stage1State.getObjectSpace().toParentSpace(comp->getCompToStageTransform().toParentSpace());
			}

			//Tanks
			for (unsigned char i = 0; i < 2; i++) {
				IStageComponent* comp = mDataSource.getStage1().getPropellantSupplies().getComponent(i);
				mMarkedLocations.tanksCoM[i] = stage1State.getObjectSpace().toParentSpace(comp->getMass(IStageComponent::CoordSpace::stage).getCentre());
				mMarkedLocations.tanksOrigins[i] = stage1State.getObjectSpace().toParentSpace(comp->getCompToStageTransform().toParentSpace());
			}
		}

		//Stage_2
		{
			const State &stage2State = mDataSource.getStage2().getState();
			mMarkedLocations.S2CentreMass = stage2State.getObjectSpace().toParentSpace(stage2State.getMass_local().getCentre());
			mMarkedLocations.S2Origin = stage2State.getObjectSpace().toParentSpace();
		}
	}

}