#include "LightSystem.h"
#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <ILightSceneNode.h>
#include <IBillboardSceneNode.h>
#include <vector3d.h>

//TODO: Do a better job with this lighting system, it is quite basic at the moment

namespace Graphics {

	SimLight::SimLight(const chrono::ChVector<> pos_world, irr::scene::ILightSceneNode& light) :
		mPosition_world(pos_world),
		mInternalLight(&light)
	{ }

	void SimLight::update(const chrono::ChVector<>& currentCamPos_world) {
		const chrono::ChVector<> displacement = mPosition_world - currentCamPos_world;
		mInternalLight->setPosition({displacement.x(), displacement.y(), displacement.z()});
	}

	LightSystem::LightSystem(irr::video::IVideoDriver& vidDriver, irr::scene::ISceneManager& sceneManager) :
		mVidDriver(vidDriver),
		mSceneManager(sceneManager)
	{
		using namespace irr;

		//Static lights
		mSceneManager.setAmbientLight({0.94f, 0.94f, 1.0f, 1.0f});

		addPointLight({10, -40, 10}, {0.992156863f, 0.647058824f, 0.058823529f, 1.0f}, 20.0f);
		addPointLight({-10, -40, 10}, {0.992156863f, 0.647058824f, 0.058823529f, 1.0f}, 20.0f);
		addPointLight({10, -40, -10}, {0.992156863f, 0.647058824f, 0.058823529f, 1.0f}, 20.0f);
		addPointLight({-10, -40, -10}, {0.992156863f, 0.647058824f, 0.058823529f, 1.0f}, 20.0f);
		
		setDirectionalLight({45.0f, 0.0f, 0.0f});
	}

	void LightSystem::update(const chrono::ChVector<>& currentCamPos_world) {
		for(SimLight& l : mLights)
			l.update(currentCamPos_world);
	}

	void LightSystem::addPointLight(const chrono::ChVector<>& pos_world, const irr::video::SColorf& colour, float radius) {
		SimLight newLight = {pos_world, *mSceneManager.addLightSceneNode(nullptr, {0, 0, 0}, colour, radius)};
		mLights.push_back(newLight);
	}

	void LightSystem::setDirectionalLight(const irr::core::vector3df eulerAngles, const irr::video::SColorf& colour) {
		irr::scene::ILightSceneNode* l = mSceneManager.addLightSceneNode(0);
		l->setLightType(irr::video::ELT_DIRECTIONAL);
		l->setRotation(eulerAngles);
	}

}