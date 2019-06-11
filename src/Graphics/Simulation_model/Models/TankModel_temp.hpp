#ifndef GRAPHICS_TANKMODELTEMP_H
#define GRAPHICS_TANKMODELTEMP_H
#pragma once

#include "../Cameras/AllCameras.h"
#include "../../../Physics/Internal/Hardware/Falcon_9/Stage_1/F9S1.h"
#include "../../GUI/UIUtils.hpp"

#include <ISceneManager.h>
#include <IMeshSceneNode.h>
#include <IMGUI/imgui.h>

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
			//chrono::ChFrame<> tankFrame = f9s1.getTankToWorldTransform();
//
			//const chrono::Vector pos_ogl = tankFrame.GetPos() - currentCamPos_world;
			//mMesh->setPosition({pos_ogl.x(), pos_ogl.y(), pos_ogl.z()});
//
			//const chrono::Vector rot_ogl = tankFrame.GetRot().Q_to_Euler123() * chrono::CH_C_RAD_TO_DEG;
			//mMesh->setRotation({rot_ogl.x(), rot_ogl.y(), rot_ogl.z()});
//
			//ImGui::Begin("TankMode_temp info");
			//ImGui::Text("abs pos: %.3f, %.3f, %.3f\n", tankFrame.GetPos().x(), tankFrame.GetPos().y(), tankFrame.GetPos().z());
			//ImGui::Text("rot    : %.3f, %.3f, %.3f\n", rot_ogl.x(), rot_ogl.y(), rot_ogl.z());
			//fixWinInViewport_imgui();
			//ImGui::End();
		}

	};

}

#endif // GRAPHICS_TANKMODELTEMP_H
