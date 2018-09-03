#include "SimulationModelLayer.h"
#include "Physics/Hardware/Falcon_9/Falcon9.h"

#include <GraphicsFramework/Vendor/ImGui/imgui.h>

#define WIREFRAME_RENDERING 0

namespace Graphics {

	SimulationModelLayer::SimulationModelLayer(Physics::Hardware::Falcon9& simDataSource, float startWindowAspect) :
		mDataSource(simDataSource),
		mCameras(simDataSource.getStage1().getState(), startWindowAspect)
	{ 
		load();
	}

	void SimulationModelLayer::render(float windowAspect, float dt) {
		mCameras.update(windowAspect, dt, mDataSource.getStage1().getState().getCMPosition_world());
		
		SimulationCamera& currentSimCamera = mCameras.getCurrentSimCamera();
		GF::Camera& currentCamera = currentSimCamera.getInternalCamera_mutable();
		mSolidRenderer.setCamera(currentCamera);
		mWireframeRenderer.setCamera(currentCamera);

		glm::dvec3 currentCamPos = currentSimCamera.getPosition();
		mASDSModel->render(currentCamPos);
		mF9S1Model->render(currentCamPos);
		mF9S2Model->render(currentCamPos);

		flushRenderers();
		
		mEarthModel->render(currentSimCamera, mDataSource.getSurfaceLocation().getEUN_to_ECEFTransform());
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

		GF::Camera& currentCamera = mCameras.getCurrentSimCamera().getInternalCamera_mutable();
		mSolidRenderer.setCamera(currentCamera);
		mWireframeRenderer.setCamera(currentCamera);
	}

	void SimulationModelLayer::flushRenderers() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mSolidRenderer.flush();
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0f);
		mWireframeRenderer.flush();
	}

}