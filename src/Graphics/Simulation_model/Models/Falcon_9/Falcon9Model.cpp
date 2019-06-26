#include "Falcon9Model.h"

namespace Graphics {

	Falcon9Model::Falcon9Model(irr::scene::ISceneManager& sceneManager) :
        mS1RP1TankModel(sceneManager, "Falcon9Full.obj")
    { }

	void Falcon9Model::update(const chrono::ChVector<>& currentCamPos_world, const ModelKeyFrame& falcon9KeyFrame) 
        // The falcon9KeyFrame passed in should contain world transforms for all the components on the vehicle.
        // This class (Falcon9Model) is responsible for applying these transforms to the component models.
    {
		// Update tank models
		// todo: Get the actual position of the tank
        mS1RP1TankModel.update(currentCamPos_world, chrono::ChCoordsys<double>(chrono::ChVector<>()));

        // Update landing leg models
        
	}

}