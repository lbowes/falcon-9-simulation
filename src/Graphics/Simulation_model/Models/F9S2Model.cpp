#include "F9S2Model.h"
#include "Physics/Hardware/Falcon_9/Stage_2/Falcon9Stage2.h"

namespace Graphics {

	F9S2Model::F9S2Model(Physics::Hardware::Falcon9Stage2& stage2Data, GF::Graphics::Renderer& renderer, GF::ResourceSet& resourceBucket) :
		ISimulationModel(renderer, resourceBucket),
		mStage2Data(stage2Data)
	{
		loadResources();
	}

	void F9S2Model::loadResources() {
		//Relies on this shader having been already added by the F9S1Model
		GF::Graphics::Shader* shader = mResourceBucket.addShader("bodyShader", "res/shaders/body.vert", "res/shaders/body.frag");
		mModel.addMesh(mResourceBucket.addOBJMesh("F9S2Fuselage", "res/models/F9S2Fuselage.obj", GL_TRIANGLES, nullptr, shader));

		addComponentModels();
	}

	void F9S2Model::addComponentModels() {
		using namespace Physics::Hardware;

		//Engines
		mComponentModels.push_back(std::make_unique<EngineMesh>("Merlin1DVac", "res/models/Merlin1DVac.obj", *static_cast<Engine*>(mStage2Data.getEngines().getComponent(0)), mResourceBucket, mModel));
	}

	void F9S2Model::update(glm::mat4 totalStageTransform_OGL) {
		//Updates the mesh representing the fuselage
		mModel.getMesh(0)->setModelTransform(totalStageTransform_OGL);

		//Updates all the component models (that, in turn, update the meshes
		//representing the landing legs, grid fins, engines etc).
		for (const auto& componentModel : mComponentModels)
			componentModel->updateResources(totalStageTransform_OGL);
	}

	void F9S2Model::updateAllTransforms_OGL(SimpleCameraState currentCameraState) {
		using namespace glm;

		const State& state = mStage2Data.getState();

		mSimpleCameraState.mPosition_highP = state.getObjectSpace().toParentSpace();
		mSimpleCameraState.mOrientation_highP = state.getOrientation_world();

		mat4
			posTransform_OGL = translate(currentCameraState.mPosition_highP - mSimpleCameraState.mPosition_highP),
			rotTransform_OGL = toMat4(inverse(mSimpleCameraState.mOrientation_highP)),
			totalTransform_OGL = inverse(rotTransform_OGL * posTransform_OGL);

		update(totalTransform_OGL);
	}

	void F9S2Model::makeRenderCalls() {
		mModel.sendRenderCommands(mRenderer);
	}

}
