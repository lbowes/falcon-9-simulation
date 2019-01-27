#include "F9S1Model.h"
#include "../../../Cameras/AllCameras.h"
#include "../../../../../Physics/Internal/Hardware/Falcon_9/Stage_1/F9S1.h"

#include <ISceneManager.h>
#include <IMeshSceneNode.h>

namespace Graphics {

	F9S1Model::F9S1Model(irr::scene::ISceneManager& sceneManager, irr::scene::ISceneNode& f9ModelSceneNode) :
		mSceneManager(sceneManager),
		mParentSceneNode(f9ModelSceneNode)
	{
		using namespace irr;
		
		scene::IAnimatedMesh* m = mSceneManager.getMesh("../res/models/F9S1Fuselage.obj");
    	mMesh = mSceneManager.addMeshSceneNode(m, &mParentSceneNode);
		mMesh->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	}

	void F9S1Model::update(const chrono::Vector& currentCamPos_world, const Physics::F9S1_DSS& f9s1, float dt) {
		const chrono::ChFrame<> f9s1Frame = f9s1.getS1ToWorldTransform();

		//This high (double) precision vector is used to eliminate floating point errors with normal low precision (float)
		//vectors. If we keep the 'internal OpenGL' camera at the origin, and calculate a high precision displacement
		//between the object's and camera's imaginary positions, then floating point errors only occur on objects very far 
		//away from the camera (where they are not noticeable anyway).
		const chrono::Vector displacement_world = f9s1Frame.GetPos() - currentCamPos_world;
		
		mMesh->setPosition(irr::core::vector3df(displacement_world.x(), displacement_world.y(), displacement_world.z()));
	}

}