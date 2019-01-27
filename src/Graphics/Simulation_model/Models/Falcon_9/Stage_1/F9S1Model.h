#ifndef GRAPHICS_F9S1MODEL_H
#define GRAPHICS_F9S1MODEL_H
#pragma once

#include <core/ChVector.h>

namespace irr {
	namespace scene {
		class ISceneManager;
		class ISceneNode;
		class IMeshSceneNode;
	}
}

namespace Graphics {
	
	class F9S1Model {
	private:
		irr::scene::ISceneManager& mSceneManager;
		irr::scene::ISceneNode& mParentSceneNode;
		irr::scene::IMeshSceneNode* mMesh;

	public:
		F9S1Model(irr::scene::ISceneManager& sceneManager, irr::scene::ISceneNode& f9ModelSceneNode);
		~F9S1Model() = default;

		void update(const chrono::ChVector<double>& currentCamPos_world, float dt);

	};

}

#endif
