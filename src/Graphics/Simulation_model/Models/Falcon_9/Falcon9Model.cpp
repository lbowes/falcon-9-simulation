#include "Falcon9Model.h"
#include "../../ModelKeyFrame.h"

namespace Graphics {

	Falcon9Model::Falcon9Model(irr::scene::ISceneManager& sceneManager) :
        mS1RP1TankModel(sceneManager, "TankModel_temp.obj")
    { }

    //todo: The whole model keyframe should not be passed into here - it should be broken down into and composed of sub-keyframe objects
    // So this function would take in a Falcon9KeyFrame for example
	void Falcon9Model::update(const chrono::ChVector<>& currentCamPos_world, const ModelKeyFrame& falcon9KeyFrame) 
        // The falcon9KeyFrame passed in should contain world transforms for all the components on the vehicle.
        // This class (Falcon9Model) is responsible for applying these transforms to the component models.
    {
		// Update tank models
		// todo: this transform should not be directly accessable to everyone (public)
        mS1RP1TankModel.update(currentCamPos_world, falcon9KeyFrame.mS1RP1TankFrame_world);

        // Update landing leg models
        
	}

}