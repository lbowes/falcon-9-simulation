#include "SimulationModelLayer.h"
#include "../../Input/HWEventReceiver.h"
#include "Cameras/AllCameras.h"
#include <ISceneNode.h>
#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <chrono_irrlicht/ChIrrTools.h>
#include <chrono_irrlicht/ChIrrWizard.h>

namespace Graphics {

	SimulationModelLayer::SimulationModelLayer(irr::video::IVideoDriver& vidDriver, irr::scene::ISceneManager& sceneManager, float windowAspect) :
		mVidDriver(vidDriver),
		mSceneManager(sceneManager)
	{ 
		mLightSystem = std::make_unique<LightSystem>(vidDriver, sceneManager);
		mF9Model = std::make_unique<F9Model>(sceneManager);
	}

	void SimulationModelLayer::update(const chrono::Vector& currentCamPos_world, const Physics::F9_DSS& f9, float dt) {
		mF9Model->update(currentCamPos_world, f9, dt);
		mLightSystem->update(currentCamPos_world);
	}

	void SimulationModelLayer::render(const chrono::Vector& currentCamPos_world) {
		mSceneManager.drawAll();
		drawGrid(currentCamPos_world);
	}

	void SimulationModelLayer::drawGrid(const chrono::Vector& currentCamPos_world) {
		using namespace chrono;
		
		//See F9S1Model::update() for explanation
		const chrono::Vector 
			gridPosition_world = {0, 0, 0},
			displacement_world = gridPosition_world - currentCamPos_world;

		irrlicht::ChIrrTools::drawGrid(
			&mVidDriver, 
			1, 
			1, 
			50, 
			50,
			ChCoordsys<>(displacement_world, Q_from_AngAxis(CH_C_PI / 2, VECT_X)),
            irr::video::SColor(70, 100, 100, 100), 
			true
		);
	}

}