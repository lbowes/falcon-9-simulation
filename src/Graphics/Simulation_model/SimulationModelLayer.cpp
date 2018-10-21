#include "Graphics/Simulation_model/SimulationModelLayer.h"
#include "Physics/Hardware/Falcon_9/Falcon9.h"

#include <GraphicsFramework/Vendor/ImGui/imgui.h>

#define WIREFRAME_RENDERING 0

namespace Graphics {

	SimulationModelLayer::SimulationModelLayer(const Physics::Hardware::Falcon9& dataSource, float startWindowAspect) :
		mDataSource(dataSource),
		mCameras(startWindowAspect)
	{ 
		load();
	}

	void SimulationModelLayer::render(float windowAspect, float dt) {
		using namespace Physics::External;
		
		const State& s1State = mDataSource.getStage1().getState();
		const CoordTransform3D& stage1ToWorld = s1State.getObjectSpace();

		mCameras.update(stage1ToWorld, windowAspect, dt, s1State.getCoMPosition_world());
		
		SimulationCamera& currentSimCamera = mCameras.getCurrentSimCamera();
		GF::Camera& currentCamera = currentSimCamera.getInternalCamera();
		mSolidRenderer.setCamera(currentCamera);
		mWireframeRenderer.setCamera(currentCamera);

		//Render objects
		glm::dvec3 currentCamPos = currentSimCamera.getPosition();
		mASDSModel->render(currentCamPos);
		mF9S1Model->render(currentCamPos);
		mF9S2Model->render(currentCamPos);
		flushRenderers();

		//Render Earth
		mEarthModel->render(currentSimCamera, mDataSource.getInertialPosition().getEUN_to_ECEFTransform());

		//Render exhaust models
		float airPressure_percent = static_cast<float>(Environment::getAirPressure_Pa(floor(s1State.getCoMPosition_world().y))) / Environment::seaLevelStdPressure;
		glm::vec3 ambientFlow_stage = -stage1ToWorld.toLocalSpace_rotation(s1State.getVelocity_world());

		for (const auto& exhaustJet : mStage1Exhausts) {
			exhaustJet->update(airPressure_percent, ambientFlow_stage);
			exhaustJet->updateTransform(mF9S1Model->getTotalTransform_OGL());
			exhaustJet->render(currentCamera);
		}
	}
	
	void SimulationModelLayer::checkInput(float dt, glm::vec2 windowDimensions) {
		using namespace GF;

		//Hide/show cursor for menu interaction/camera focus respectively 
		if (Input::isMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT) && mCameras.hasFocus()) {
			Input::showCursor();
			Input::setMousePosition(windowDimensions / 2.0f);
			mCameras.shouldHaveFocus(false);
		}
		if (Input::isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse) {
			Input::hideCursor();
			mCameras.shouldHaveFocus(true);
		}

		mCameras.checkInput(dt);
	}

	void SimulationModelLayer::load() {
		using namespace std;
		using namespace GF::Graphics;

		//glPointSize(4.0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		mF9S1Model = make_unique<F9S1Model>(mDataSource.getStage1(), WIREFRAME_RENDERING ? mWireframeRenderer : mSolidRenderer, mResourceBucket);
		mF9S2Model = make_unique<F9S2Model>(mDataSource.getStage2(), WIREFRAME_RENDERING ? mWireframeRenderer : mSolidRenderer, mResourceBucket);
		mASDSModel = make_unique<ASDSModel>(WIREFRAME_RENDERING ? mWireframeRenderer : mSolidRenderer, mResourceBucket, *mResourceBucket.getResource<Shader>("bodyShader"));
		mEarthModel = make_unique<EarthModel>(mResourceBucket);

		const Physics::Hardware::ThrustGeneratorGroup& S1Engines = mDataSource.getStage1().getEngines();
		for (const auto& engine : S1Engines.getAllComponents())
			mStage1Exhausts.push_back(std::make_unique<ExhaustJet>(*static_cast<Physics::Hardware::Engine*>(engine.get()), S1Engines, mResourceBucket));

		GF::Camera& currentCamera = mCameras.getCurrentSimCamera().getInternalCamera();
		mSolidRenderer.setCamera(currentCamera);
		mWireframeRenderer.setCamera(currentCamera);
	}

	void SimulationModelLayer::flushRenderers() {
		mSolidRenderer.flush();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0f);
		mWireframeRenderer.flush();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}