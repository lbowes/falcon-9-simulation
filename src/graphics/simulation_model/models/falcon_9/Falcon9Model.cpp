#include "Falcon9Model.h"
#include "../../Falcon9Keyframe.h"

namespace Graphics {

	Falcon9Model::Falcon9Model(irr::scene::ISceneManager& sceneManager) {
        addComponentModels(sceneManager);
    }

	void Falcon9Model::update(const chrono::ChVector<>& currentCamPos_world, const Falcon9Keyframe& falcon9Keyframe)
        // The falcon9Keyframe passed in should contain world transforms for all the components on the vehicle.
        // This class (Falcon9Model) is responsible for applying these transforms to the component models.
    {
        for(auto& c : mComponentModels) {
            const chrono::ChCoordsys<double>* transform = falcon9Keyframe.getCoordSystem(c.first);

            if(transform != nullptr)
                c.second->update(currentCamPos_world, *transform);
        }
	}

    void Falcon9Model::addComponentModels(irr::scene::ISceneManager& sceneManager) {
        mComponentModels["S1RP1Tank"] = std::make_unique<OBJModel>(sceneManager, "TankModel_temp.obj");
        mComponentModels["LandingLeg_0"] = std::make_unique<OBJModel>(sceneManager, "LandingLeg.obj");
    }

}