#ifndef GRAPHICS_F9S1MODEL_H
#define GRAPHICS_F9S1MODEL_H
#pragma once

#include "../OBJModel.hpp"

#include <core/ChVector.h>

namespace irr {
	namespace scene {
		class ISceneManager;
	}
}

namespace Graphics {

    class ModelKeyFrame;

	class Falcon9Model {
	private:
		OBJModel 
            mS1RP1TankModel;
            // stage 1 merlins,
            // stage 2 merlin
            // legs,
            // grid fins, 
            // payload fairings,
            // stage 1 fuselage
            // stage 2 fuselage

	public:
		Falcon9Model(irr::scene::ISceneManager& sceneManager);
		~Falcon9Model() = default;

		void update(const chrono::ChVector<>& currentCamPos_world, const ModelKeyFrame& k);

	};

}

#endif // GRAPHICS_F9S1MODEL_H
