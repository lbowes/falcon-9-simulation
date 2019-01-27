#include "F9S2Model.h"
#include <ISceneManager.h>
#include <IMeshSceneNode.h>
#include <core/ChVector.h>

namespace Graphics {

	F9S2Model::F9S2Model(irr::scene::ISceneManager& sceneManager, irr::scene::ISceneNode& f9ModelSceneNode) :
		mSceneManager(sceneManager),
		mParentSceneNode(f9ModelSceneNode)
	{
		using namespace irr;
		
		scene::IAnimatedMesh* m = mSceneManager.getMesh("../res/models/F9S2FuselageFull.obj");
    	mMesh = mSceneManager.addMeshSceneNode(m, &mParentSceneNode);
		mMesh->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	}

	void F9S2Model::update(const chrono::ChVector<double>& currentCamPos_world, float dt) {
		//See F9S1Model::update() for explanation
		const chrono::Vector
			objectPosition_world = {10, 0, 0}, //TODO: eventually source position from data source rigid body in simulation
			displacement_world = objectPosition_world - currentCamPos_world;
		
		mMesh->setPosition(irr::core::vector3df(displacement_world.x(), displacement_world.y(), displacement_world.z()));
	}

}