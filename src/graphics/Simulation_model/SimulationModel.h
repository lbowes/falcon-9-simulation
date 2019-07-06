#ifndef GRAPHICS_SIMULATIONMODEL_H
#define GRAPHICS_SIMULATIONMODEL_H
#pragma once

#include "Lighting/LightSystem.h"
#include "Models/Falcon_9/Falcon9Model.h"
#include "Models/GroundModel.h"
#include "Falcon9Keyframe.h"

#include <memory>

namespace irr {
	namespace scene {
		class ISceneNode;
		class ISceneManager;
		class IMeshSceneNode;
	}
	namespace video {
		class IVideoDriver;
	}
}

namespace Graphics {

	class SimulationModel {
	private:
		irr::scene::ISceneManager& mSceneManager;
		
		LightSystem mLightSystem;
        Falcon9Model mFalcon9Model;
		GroundModel mGroundModel;

	public:
		SimulationModel(irr::video::IVideoDriver& vidDriver, irr::scene::ISceneManager& sceneManager);
		~SimulationModel() = default;

		void update(const chrono::Vector& currentCamPos_world, const Falcon9Keyframe& Keyframe);
		void render(const chrono::Vector& currentCamPos_world);

	};

}

#endif // GRAPHICS_SIMULATIONMODEL_H
