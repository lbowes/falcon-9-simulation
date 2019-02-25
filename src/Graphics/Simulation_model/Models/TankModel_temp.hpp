#ifndef GRAPHICS_TANKMODELTEMP_H
#define GRAPHICS_TANKMODELTEMP_H
#pragma once

#include "../Cameras/AllCameras.h"
#include "../../../Physics/Internal/Hardware/Falcon_9/Stage_1/F9S1.h"

#include <ISceneManager.h>
#include <IMeshSceneNode.h>

namespace Graphics {
	
	class TankModel_temp {
	private:
		irr::scene::ISceneManager& mSceneManager;
		irr::scene::IMeshSceneNode* mMesh;

	public:
		TankModel_temp(irr::scene::ISceneManager& sceneManager) :
			mSceneManager(sceneManager)
		{
			using namespace irr;

			scene::IAnimatedMesh* m = mSceneManager.getMesh("../res/models/TankModel_temp.obj");
    		mMesh = mSceneManager.addMeshSceneNode(m);
			mMesh->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		}
	
		~TankModel_temp() = default;

		void update(const chrono::ChVector<double>& currentCamPos_world, const Physics::F9S1_DSS& f9s1, float dt) {
			chrono::ChFrame<> tankFrame = f9s1.getTankToWorldTransform();

			const chrono::Vector pos_world = tankFrame.GetPos() - currentCamPos_world;
			mMesh->setPosition({pos_world.x(), pos_world.y(), pos_world.z()});

			const chrono::Vector rot_world = tankFrame.GetRot().Q_to_Euler123() * chrono::CH_C_RAD_TO_DEG;
			mMesh->setRotation({rot_world.x(), rot_world.y(), rot_world.z()});
		}

	};

}

#endif
