#ifndef GRAPHICS_F9S1MODEL_H
#define GRAPHICS_F9S1MODEL_H
#pragma once

#include "S1RP1TankModel.hpp"

#include <core/ChVector.h>

namespace irr {
	namespace scene {
		class ISceneManager;
		class ISceneNode;
		class IMeshSceneNode;
	}
}

namespace Physics {
	class Falcon9_DSS;
}

namespace Graphics {
	
	class Falcon9Model {
	private:
		std::unique_ptr<S1RP1TankModel> mS1RP1TankModel;

	public:
		Falcon9Model(irr::scene::ISceneManager& sceneManager);
		~Falcon9Model() = default;

		void update(const chrono::ChVector<>& currentCamPos_world, const Physics::Falcon9_DSS& f9, float dt);

	};

}

#endif // GRAPHICS_F9S1MODEL_H
