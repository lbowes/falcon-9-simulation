#include "F9Model.h"
#include "../../../../Physics/Internal/Hardware/Falcon_9/F9.h"

#include <ISceneManager.h>
#include <IMeshSceneNode.h>
#include <core/ChVector.h>

namespace Graphics {

	F9Model::F9Model(irr::scene::ISceneManager& sceneManager) :
		mSceneManager(sceneManager)
	{
		using namespace irr;

		irr::scene::ISceneNode* f9RootSceneNode = mSceneManager.addEmptySceneNode();
		mS1Model = std::make_unique<F9S1Model>(sceneManager, *f9RootSceneNode);
		//mS2Model = std::make_unique<F9S2Model>(sceneManager, *f9RootSceneNode);
	}

	void F9Model::update(const chrono::Vector& currentCamPos_world, const Physics::F9_DSS& f9, float dt) {
		mS1Model->update(currentCamPos_world, f9.getF9S1_DSS(), dt);
		//mS2Model->update(currentCamPos_world, dt);
	}

}