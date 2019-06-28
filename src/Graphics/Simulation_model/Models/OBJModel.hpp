#ifndef GRAPHICS_OBJMODEL_HPP
#define GRAPHICS_OBJMODEL_HPP
#pragma once

#include <ISceneManager.h>
#include <IMeshSceneNode.h>
#include <chrono_irrlicht/ChIrrTools.h>

namespace Graphics {
	
	class OBJModel {
    private:
        static constexpr auto sModelPath = "../res/models/";
		irr::scene::IMeshSceneNode* mMesh;

	public:
		OBJModel(irr::scene::ISceneManager& sceneManager, const std::string& objFilename) {
			using namespace irr;

            const std::string objModelFilepath = sModelPath + objFilename;
    		mMesh = sceneManager.addMeshSceneNode(sceneManager.getMesh(objModelFilepath.c_str()));
			mMesh->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);

            // Any other material flags that are to be set for OBJ models should be set here
            // mMesh->setMaterialFlag(..., ...);
            // e.g. mMesh->setMaterialFlag(video::EMF_WIREFRAME, true);
		}
	
		~OBJModel() = default;

		void update(const chrono::ChVector<>& camPos_world, chrono::ChCoordsys<> transform_world) 
            // This high (double) precision vector is used to eliminate floating point errors with normal low precision (float)
		    // vectors. If we keep the 'internal OpenGL' camera at the origin, and calculate a high precision displacement
		    // between the object's and camera's imaginary positions, then floating point errors only occur on objects very far 
		    // away from the camera (where they are not noticeable anyway).
        {
			transform_world.pos -= camPos_world;
            chrono::irrlicht::ChIrrTools::alignIrrlichtNodeToChronoCsys(mMesh, transform_world);
		}

	};

}

#endif // GRAPHICS_OBJMODEL_HPP
