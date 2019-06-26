#include "Falcon9Model.h"

namespace Graphics {

	Falcon9Model::Falcon9Model(irr::scene::ISceneManager& sceneManager) :
        mS1RP1TankModel(sceneManager, "TankModel_temp.obj")
    { }

	void Falcon9Model::update(const chrono::ChVector<>& currentCamPos_world, const ModelKeyFrame& falcon9KeyFrame) {
		// Update tank models
		// todo: Get the actual position of the tank
        mS1RP1TankModel.update(currentCamPos_world, chrono::ChCoordsys<double>(chrono::ChVector<>(0, -7, 0)));

        // Update landing leg models
        
	}

}