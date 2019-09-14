#ifndef F9SIM_GRAPHICS_SIMULATIONMODEL_H_
#define F9SIM_GRAPHICS_SIMULATIONMODEL_H_
#pragma once

#include "lighting/LightSystem.h"
#include "models/falcon_9/Falcon9Model.h"
#include "models/GroundModel.h"
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

#endif // F9SIM_GRAPHICS_SIMULATIONMODEL_H_
