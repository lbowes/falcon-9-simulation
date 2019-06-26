#ifndef GRAPHICS_LIGHTSYSTEM_H
#define GRAPHICS_LIGHTSYSTEM_H
#pragma once

#include <core/ChVector.h>
#include <vector3d.h>
#include <SColor.h>

namespace irr {
	namespace scene {
		class ISceneNode;
		class ISceneManager;
		class ILightSceneNode;
	}
	namespace video {
		class IVideoDriver;
	}
}

namespace Graphics {

	class SimLight {
	private:
		chrono::ChVector<> mPosition_world;
		irr::scene::ILightSceneNode* mInternalLight; 

	public:
		SimLight(const chrono::ChVector<> pos_world, irr::scene::ILightSceneNode& light);
		~SimLight() = default;

		void update(const chrono::ChVector<>& currentCamPos_world);
		
		void setPosition_world(const chrono::ChVector<>& pos_world) { mPosition_world = pos_world; }

	};

	class LightSystem {
	private:
		irr::scene::ISceneManager& mSceneManager;

		std::vector<SimLight> mLights;

	public:
		LightSystem(irr::scene::ISceneManager& sceneManager);
		~LightSystem() = default;

		void update(const chrono::ChVector<double>& currentCamPos_world);
		void addPointLight(const chrono::ChVector<>& pos_world = chrono::ChVector<>(0), const irr::video::SColorf& colour = irr::video::SColorf(1.0f), float radius = 100.0f);
		void setDirectionalLight(const irr::core::vector3df eulerAngles, const irr::video::SColorf& colour = irr::video::SColorf(1.0f));

	};

}

#endif // GRAPHICS_LIGHTSYSTEM_H
