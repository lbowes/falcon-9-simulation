#ifndef GRAPHICS_S1RP1TANKMODEL_H
#define GRAPHICS_S1RP1TANKMODEL_H
#pragma once

#include "../../../Cameras/AllCameras.h"

#include <ISceneManager.h>
#include <IMeshSceneNode.h>
#include <chrono_irrlicht/ChIrrTools.h>

namespace Graphics {
	
	class S1RP1TankModel {
	private:
		irr::scene::ISceneManager& mSceneManager;
		irr::scene::IMeshSceneNode* mMesh;

	public:
		S1RP1TankModel(irr::scene::ISceneManager& sceneManager) :
			mSceneManager(sceneManager)
		{
			using namespace irr;

			scene::IAnimatedMesh* m = mSceneManager.getMesh("../res/models/TankModel_temp.obj");
    		mMesh = mSceneManager.addMeshSceneNode(m);
			mMesh->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		}
	
		~S1RP1TankModel() = default;

		void update(const chrono::ChVector<>& currentCamPos_world, const chrono::ChCoordsys<>& tankTransform_world, float dt) {
			const chrono::Vector pos_ogl = tankTransform_world.pos - currentCamPos_world;
			mMesh->setPosition(irr::core::vector3dfCH(pos_ogl));

            const chrono::Vector rot_ogl = tankTransform_world.rot.Q_to_Euler123() * chrono::CH_C_RAD_TO_DEG;
        	mMesh->setRotation(irr::core::vector3dfCH(rot_ogl));
		}

	};

}

#endif // GRAPHICS_TANKMODELTEMP_H
