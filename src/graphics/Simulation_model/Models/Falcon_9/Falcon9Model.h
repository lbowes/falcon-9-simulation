#ifndef GRAPHICS_F9S1MODEL_H
#define GRAPHICS_F9S1MODEL_H
#pragma once

#include "../OBJModel.hpp"

#include <core/ChVector.h>
#include <map>

namespace irr {
	namespace scene {
		class ISceneManager;
	}
}

namespace Graphics {

    class Falcon9Keyframe;

	class Falcon9Model {
	private:
        std::map<std::string, std::unique_ptr<OBJModel>> mComponentModels;

	public:
		Falcon9Model(irr::scene::ISceneManager& sceneManager);
		~Falcon9Model() = default;

		void update(const chrono::ChVector<>& currentCamPos_world, const Falcon9Keyframe& k);
        
    private:
        void addComponentModels(irr::scene::ISceneManager& sceneManager);

	};

}

#endif // GRAPHICS_F9S1MODEL_H
