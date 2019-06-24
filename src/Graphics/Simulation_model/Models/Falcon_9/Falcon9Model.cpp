#include "Falcon9Model.h"
#include "../../../Cameras/AllCameras.h"

#include <ISceneManager.h>

namespace Graphics {

	Falcon9Model::Falcon9Model(irr::scene::ISceneManager& sceneManager) {
		//using namespace irr;
		//scene::IAnimatedMesh* m = mSceneManager.getMesh("../res/models/F9S1Fuselage.obj");
    	//mMesh = mSceneManager.addMeshSceneNode(m);
		//mMesh->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		//mMesh->setMaterialFlag(video::EMF_WIREFRAME, true);

		mS1RP1TankModel = std::make_unique<S1RP1TankModel>(sceneManager);
	}

	void Falcon9Model::update(const chrono::ChVector<>& currentCamPos_world, const Physics::Falcon9_DSS& f9, float dt) 
        // This high (double) precision vector is used to eliminate floating point errors with normal low precision (float)
		// vectors. If we keep the 'internal OpenGL' camera at the origin, and calculate a high precision displacement
		// between the object's and camera's imaginary positions, then floating point errors only occur on objects very far 
		// away from the camera (where they are not noticeable anyway).
    {
		chrono::ChCoordsys<> f9s1Frame = f9.getS1ToWorldTransform().GetCoord();

		

		const chrono::Vector pos_ogl = f9s1Frame.pos - currentCamPos_world;
		mMesh->setPosition({pos_ogl.x(), pos_ogl.y(), pos_ogl.z()});

		const chrono::Vector rot_ogl = f9s1Frame.rot.Q_to_Euler123() * chrono::CH_C_RAD_TO_DEG;
        mMesh->setRotation({rot_ogl.x(), rot_ogl.y(), rot_ogl.z()});
        //mMesh->setRotation({rot[0], rot[1], rot[2]});

		// temp
		mS1RP1TankModel->update(currentCamPos_world, f9s1, dt);
		//
	}

}