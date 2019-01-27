#ifndef GRAPHICS_F9MODEL_H
#define GRAPHICS_F9MODEL_H
#pragma once

#include "Stage_1/F9S1Model.h"
#include "Stage_2/F9S2Model.h"
#include <memory>
#include <core/ChVector.h>

namespace irr {
	namespace scene {
		class ISceneManager;
	}
}

namespace Physics {
	class F9_DSS;
}

namespace Graphics {
	
	class F9Model {
	private:
		irr::scene::ISceneManager& mSceneManager;
		std::unique_ptr<F9S1Model> mS1Model;
		//std::unique_ptr<F9S2Model> mS2Model;

	public:
		F9Model(irr::scene::ISceneManager& sceneManager);
		~F9Model() = default;

		void update(const chrono::ChVector<double>& currentCamPos_world, const Physics::F9_DSS& f9, float dt);
		void render();

	};

}

#endif
